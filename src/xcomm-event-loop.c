/** Copyright (c) 2025, Wu Jin <wujin.developer@gmail.com>
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to
 *  deal in the Software without restriction, including without limitation the
 *  rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 *  sell copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 *  IN THE SOFTWARE.
 */

#include "xcomm-utils.h"
#include "xcomm-event-loop.h"

#include "platform/platform-event.h"
#include "platform/platform-socket.h"

static inline int
_event_loop_minheap_cmp(xcomm_heap_node_t* a, xcomm_heap_node_t* b) {
    xcomm_event_t* event_a = xcomm_heap_data(a, xcomm_event_t, tm_node);
    xcomm_event_t* event_b = xcomm_heap_data(b, xcomm_event_t, tm_node);

    uint64_t expire_a = event_a->tm.birth + event_a->tm.expire;
    uint64_t expire_b = event_b->tm.birth + event_b->tm.expire;

    if (expire_a < expire_b) {
        return 1;
    }
    if (expire_a > expire_b) {
        return 0;
    }
    return (event_a->tm.id < event_b->tm.id) ? 1 : 0;
}

static void _event_loop_wake(xcomm_event_loop_t* loop) {
    char buf = 'w';
    platform_socket_send(loop->wakefds[0], &buf, sizeof(buf));
}

static void _event_loop_wake_cb(void* context, platform_event_op_t op) {
    (void)op;
    xcomm_event_loop_t* loop = (xcomm_event_loop_t*)context;
    char buf;
    platform_socket_recv(loop->wakefds[1], &buf, sizeof(buf));
}

static void _event_loop_process_timers(xcomm_event_loop_t* loop) {
    uint64_t now = xcomm_utils_getnow(XCOMM_TIME_PRECISION_MSEC);

    while (!xcomm_heap_empty(&loop->tm_ev_mgr)) {
        xcomm_heap_node_t* min = xcomm_heap_min(&loop->tm_ev_mgr);
        xcomm_event_t*     evt = xcomm_heap_data(min, xcomm_event_t, tm_node);

        if (evt->tm.birth + evt->tm.expire > now) {
            break;
        }
        if (evt->execute_cb) {
            evt->execute_cb(evt->context, PLATFORM_EVENT_NO_OP);
        }
    }
}

static int _event_loop_calculate_timeout(xcomm_event_loop_t* loop) {
    if (xcomm_heap_empty(&loop->tm_ev_mgr)) {
        return INT_MAX - 1;
    }

    xcomm_heap_node_t* min = xcomm_heap_min(&loop->tm_ev_mgr);
    xcomm_event_t*     evt = xcomm_heap_data(min, xcomm_event_t, tm_node);

    uint64_t now = xcomm_utils_getnow(XCOMM_TIME_PRECISION_MSEC);
    if (evt->tm.birth + evt->tm.expire <= now) {
        return 0;
    }
    return (int)(evt->tm.birth + evt->tm.expire - now);
}

void xcomm_event_loop_init(xcomm_event_loop_t* loop) {
    loop->running = true;
    loop->tid = thrd_current();
    
    mtx_init(&loop->rt_ev_mtx, mtx_plain);
    
    xcomm_queue_init(&loop->rt_ev_mgr);
    loop->rt_ev_num = 0;

    xcomm_list_init(&loop->io_ev_mgr);
    loop->io_ev_num = 0;

    xcomm_heap_init(&loop->tm_ev_mgr, _event_loop_minheap_cmp);
    loop->tm_ev_num = 0;

    platform_event_init(&loop->sq);
    platform_socket_socketpair(AF_INET, SOCK_STREAM, 0, loop->wakefds);
    platform_socket_enable_nonblocking(loop->wakefds[1], true);

    xcomm_event_t* event = malloc(sizeof(xcomm_event_t));
    if (!event) {
        return;
    }
    event->type = XCOMM_EVENT_TYPE_IO;
    event->context = loop;
    event->execute_cb = _event_loop_wake_cb;
    event->cleanup_cb = NULL;

    event->io.sqe.op = PLATFORM_EVENT_RD_OP;
    event->io.sqe.fd = (platform_event_fd_t)loop->wakefds[1];
    event->io.sqe.ud = event;

    xcomm_list_insert_tail(&loop->io_ev_mgr, &event->io_node);
    loop->io_ev_num++;

    platform_event_add(loop->sq, &event->io.sqe);
}

void xcomm_event_loop_register(xcomm_event_loop_t* loop, xcomm_event_t* event) {
    switch (event->type) {
    case XCOMM_EVENT_TYPE_IO: {
        xcomm_list_insert_tail(&loop->io_ev_mgr, &event->io_node);
        loop->io_ev_num++;

        platform_event_add(loop->sq, &event->io.sqe);
        break;
    }
    case XCOMM_EVENT_TYPE_RT: {
        mtx_lock(&loop->rt_ev_mtx);
        xcomm_queue_enqueue(&loop->rt_ev_mgr, &event->rt_node);
        loop->rt_ev_num++;
        mtx_unlock(&loop->rt_ev_mtx);

        _event_loop_wake(loop);
        break;
    }
    case XCOMM_EVENT_TYPE_TM: {
        xcomm_heap_insert(&loop->tm_ev_mgr, &event->tm_node);
        loop->tm_ev_num++;
        break;
    }
    default:
        break;
    }
}

void xcomm_event_loop_update(xcomm_event_loop_t* loop, xcomm_event_t* event) {
    if (event->type == XCOMM_EVENT_TYPE_IO) {
        platform_event_mod(loop->sq, &event->io.sqe);
    }
}

void xcomm_event_loop_unregister(xcomm_event_loop_t* loop, xcomm_event_t* event) {
    switch (event->type) {
    case XCOMM_EVENT_TYPE_IO: {
        xcomm_list_remove(&event->io_node);
        loop->io_ev_num--;

        platform_event_del(loop->sq, &event->io.sqe);
        break;
    }
    case XCOMM_EVENT_TYPE_RT: {
        break;
    }
    case XCOMM_EVENT_TYPE_TM: {
        xcomm_heap_remove(&loop->tm_ev_mgr, &event->tm_node);
        loop->tm_ev_num--;
        break;
    }
    default:
        break;
    }
}

void xcomm_event_loop_run(xcomm_event_loop_t* loop) {
    platform_event_cqe_t cqes[PLATFORM_EVENT_CQE_NUM] = {0};
    while (loop->running) {
        _event_loop_process_timers(loop);

        int nevents = platform_event_wait(
            loop->sq, cqes, _event_loop_calculate_timeout(loop));

        for (int i = 0; i < nevents; i++) {
            xcomm_event_t* event = cqes[i].ud;

            if (event && event->execute_cb) {
                event->execute_cb(event->context, cqes[i].op);
            }
        }
        _event_loop_process_timers(loop);
    }
}

void xcomm_event_loop_destroy(xcomm_event_loop_t* loop) {
}