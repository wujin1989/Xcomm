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

#include "platform/platform-poller.h"
#include "platform/platform-socket.h"

static void _event_loop_wake(xcomm_event_loop_t* loop) {
    char buf = 'w';
    platform_socket_send(loop->wakefds[0], &buf, sizeof(buf));
}

static void _event_loop_wake_cb(void* context, platform_poller_op_t op) {
    (void)op;
    xcomm_event_loop_t* loop = (xcomm_event_loop_t*)context;
    char                buf[64];
    while (true) {
        ssize_t n = platform_socket_recv(loop->wakefds[1], buf, sizeof(buf));
        if (n == PLATFORM_SO_ERROR_SOCKET_ERROR) {
            int err = platform_socket_get_lasterror();
            if ((err == PLATFORM_SO_ERROR_EAGAIN) ||
                (err == PLATFORM_SO_ERROR_EWOULDBLOCK)) {
                break;
            }
            break;
        }
    }
}

static int _event_loop_calculate_timeout(xcomm_event_loop_t* loop) {
    if (xcomm_heap_empty(&loop->tm_ev_mgr)) {
        return INT_MAX - 1;
    }
    xcomm_event_t* event = xcomm_heap_data(
        xcomm_heap_min(&loop->tm_ev_mgr), xcomm_event_t, tm_node);

    return event->tm.calculate_timeout_cb(event->context);
}

static void _event_loop_process_timers(xcomm_event_loop_t* loop) {
    uint64_t now = xcomm_utils_getnow(XCOMM_TIME_PRECISION_MSEC);

    mtx_lock(&loop->tm_ev_mtx);
    while (!xcomm_heap_empty(&loop->tm_ev_mgr)) {
        xcomm_event_t* event = xcomm_heap_data(
            xcomm_heap_min(&loop->tm_ev_mgr), xcomm_event_t, tm_node);
        
        if (event->tm.calculate_timeout_cb(event->context)) {
            break;
        }
        mtx_unlock(&loop->tm_ev_mtx);

        event->tm.execute_cb(event->context);
        mtx_lock(&loop->tm_ev_mtx);
    }
    mtx_unlock(&loop->tm_ev_mtx);
}

static void _event_loop_process_routines(xcomm_event_loop_t* loop) {
    xcomm_queue_t temp;
    xcomm_queue_init(&temp);

    mtx_lock(&loop->rt_ev_mtx);
    xcomm_queue_swap(&temp, &loop->rt_ev_mgr);
    loop->rt_ev_num = 0;
    mtx_unlock(&loop->rt_ev_mtx);

    while (!xcomm_queue_empty(&temp)) {
        xcomm_queue_node_t* node = xcomm_queue_dequeue(&temp);
        xcomm_event_t* event = xcomm_queue_data(node, xcomm_event_t, rt_node);

        if (event->rt.execute_cb) {
            event->rt.execute_cb(event->context);
        }
        if (event->rt.cleanup_cb) {
            event->rt.cleanup_cb(event->context);
        }
    }
}

static inline int
_event_loop_minheap_cmp(xcomm_heap_node_t* a, xcomm_heap_node_t* b) {
    xcomm_event_t* event_a = xcomm_heap_data(a, xcomm_event_t, tm_node);
    xcomm_event_t* event_b = xcomm_heap_data(b, xcomm_event_t, tm_node);

    return event_a->tm.compare_minheap_cb(event_a->context, event_b->context);
}

void xcomm_event_loop_init(xcomm_event_loop_t* loop) {
    loop->running = true;
    loop->tid = thrd_current();
    
    mtx_init(&loop->rt_ev_mtx, mtx_plain);
    mtx_init(&loop->tm_ev_mtx, mtx_plain);
    mtx_init(&loop->io_ev_mtx, mtx_plain);
    
    xcomm_queue_init(&loop->rt_ev_mgr);
    loop->rt_ev_num = 0;

    xcomm_list_init(&loop->io_ev_mgr);
    loop->io_ev_num = 0;

    xcomm_heap_init(&loop->tm_ev_mgr, _event_loop_minheap_cmp);
    loop->tm_ev_num = 0;

    platform_poller_init(&loop->sq);
    platform_socket_socketpair(AF_INET, SOCK_STREAM, 0, loop->wakefds);
    platform_socket_enable_nonblocking(loop->wakefds[1], true);

    xcomm_event_t* event = malloc(sizeof(xcomm_event_t));
    if (!event) {
        return;
    }
    event->type = XCOMM_EVENT_TYPE_IO;
    event->loop = loop;

    event->io.execute_cb = _event_loop_wake_cb;
    event->io.cleanup_cb = NULL;
    event->context       = loop;

    event->io.sqe.op = PLATFORM_POLLER_RD_OP;
    event->io.sqe.fd = (platform_poller_fd_t)loop->wakefds[1];
    event->io.sqe.ud = event;

    xcomm_list_insert_tail(&loop->io_ev_mgr, &event->io_node);
    loop->io_ev_num++;

    platform_poller_add(loop->sq, &event->io.sqe);
}

void xcomm_event_loop_destroy(xcomm_event_loop_t* loop) {

}

void xcomm_event_loop_register(xcomm_event_loop_t* loop, xcomm_event_t* event) {
    switch (event->type) {
    case XCOMM_EVENT_TYPE_IO: {
        xcomm_list_insert_tail(&loop->io_ev_mgr, &event->io_node);
        loop->io_ev_num++;

        platform_poller_add(loop->sq, &event->io.sqe);
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
        mtx_lock(&loop->tm_ev_mtx);
        xcomm_heap_insert(&loop->tm_ev_mgr, &event->tm_node);
        loop->tm_ev_num++;
        mtx_unlock(&loop->tm_ev_mtx);
        break;
    }
    default:
        break;
    }
}

void xcomm_event_loop_update(xcomm_event_loop_t* loop, xcomm_event_t* event) {
    if (event->type == XCOMM_EVENT_TYPE_IO) {
        platform_poller_mod(loop->sq, &event->io.sqe);
    }
}

void xcomm_event_loop_unregister(xcomm_event_loop_t* loop, xcomm_event_t* event) {
    switch (event->type) {
    case XCOMM_EVENT_TYPE_IO: {
        xcomm_list_remove(&event->io_node);
        loop->io_ev_num--;

        platform_poller_del(loop->sq, &event->io.sqe);
        break;
    }
    case XCOMM_EVENT_TYPE_RT: {
        /**
         * Due to the fact that RT events are processed in the event loop by
         * copying the entire RT queue at once, there is no need to handle them
         * here.
         */
        break;
    }
    case XCOMM_EVENT_TYPE_TM: {
        mtx_lock(&loop->tm_ev_mtx);
        xcomm_heap_remove(&loop->tm_ev_mgr, &event->tm_node);
        loop->tm_ev_num--;
        mtx_unlock(&loop->tm_ev_mtx);
        break;
    }
    default:
        break;
    }
}

void xcomm_event_loop_run(xcomm_event_loop_t* loop) {
    platform_poller_cqe_t cqes[PLATFORM_POLLER_CQE_NUM] = {0};

    while (loop->running) {
        _event_loop_process_routines(loop);

        int nevents = platform_poller_wait(
            loop->sq, cqes, _event_loop_calculate_timeout(loop));

        _event_loop_process_timers(loop);

        for (int i = 0; i < nevents; i++) {
            xcomm_event_t* event = cqes[i].ud;

            if (event && event->io.execute_cb) {
                event->io.execute_cb(event->context, cqes[i].op);
            }
        }
        _event_loop_process_timers(loop);
    }
}

void xcomm_event_loop_stop(xcomm_event_loop_t* loop) {
    loop->running = false;
    _event_loop_wake(loop);
}