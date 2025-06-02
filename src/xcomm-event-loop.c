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

#include "xcomm-event-loop.h"

#include "platform/platform-event.h"
#include "platform/platform-socket.h"

#define xcomm_eventloop_container_of(x, t, m) ((t*)((char*)(x)-offsetof(t, m)))

static inline int
_event_loop_minheap_cmp(xcomm_heap_node_t* a, xcomm_heap_node_t* b) {
    xcomm_event_timer_t* ta = xcomm_heap_data(a, xcomm_event_timer_t, node);
    xcomm_event_timer_t* tb = xcomm_heap_data(b, xcomm_event_timer_t, node);

    if ((ta->birth + ta->expire) < (tb->birth + tb->expire)) {
        return 1;
    }
    if ((ta->birth + ta->expire) == (tb->birth + tb->expire)) {
        if ((ta->id < tb->id)) {
            return 1;
        }
    }
    return 0;
}

static void _event_loop_wake(xcomm_eventloop_t* loop) {
    char buf = 'w';
    platform_socket_send(loop->wakefds[0], &buf, sizeof(buf));
}

static void _eventloop_wake_cb(void* context) {
    xcomm_eventloop_t* loop = (xcomm_eventloop_t*)context;

    char buf;
    platform_socket_recv(loop->wakefds[1], &buf, sizeof(buf));
}

void xcomm_eventloop_init(xcomm_eventloop_t* loop) {
    loop->running = true;
    loop->tid = thrd_current();
    
    mtx_init(&loop->rt_mtx, mtx_plain);
    
    xcomm_list_init(&loop->rt_evts);
    xcomm_rbtree_init(&loop->io_evts, xcomm_rbtree_keycmp_ptr);

    xcomm_heap_init(&loop->ev_tm_mgr, _event_loop_minheap_cmp);
    loop->ev_tm_mgr->ntimers = 0;

    platform_event_init(&loop->sq);
    platform_socket_socketpair(AF_INET, SOCK_STREAM, 0, loop->wakefds);
    platform_socket_enable_nonblocking(loop->wakefds[1], true);

    xcomm_io_event_t* event = malloc(sizeof(xcomm_io_event_t));
    if (!event) {
        return;
    }
    event->base.type       = XCOMM_EVENT_TYPE_IO;
    event->base.context    = loop;
    event->base.execute_cb = _eventloop_wake_cb;
    event->base.execute_cb = NULL;

    event->sqe.op = PLATFORM_EVENT_RD_OP;
    event->sqe.fd = (platform_event_fd_t)loop->wakefds[1];
    event->sqe.ud = event;
    event->node.key.ptr = loop;

    xcomm_rbtree_insert(&loop->io_evts, &event->node);
    platform_event_add(loop->sq, &event->sqe);
}

void xcomm_eventloop_register(xcomm_eventloop_t* loop, xcomm_event_t* event) {
    switch (event->type) {
    case XCOMM_EVENT_TYPE_IO: {
        xcomm_io_event_t* io_evt =
            xcomm_eventloop_container_of(event, xcomm_io_event_t, base);

        xcomm_rbtree_insert(&loop->io_evts, &io_evt->node);
        platform_event_add(loop->sq, &io_evt->sqe);
        break;
    }
    case XCOMM_EVENT_TYPE_RT: {
        xcomm_rt_event_t* rt_evt =
            xcomm_eventloop_container_of(event, xcomm_rt_event_t, base);

        mtx_lock(&loop->rt_mtx);
        xcomm_list_insert_tail(&loop->rt_evts, event);
        mtx_unlock(&loop->rt_mtx);

        _eventloop_wake(loop);
        break;
    }
    case XCOMM_EVENT_TYPE_TM: {
        xcomm_tm_event_t* tm_evt =
            xcomm_eventloop_container_of(event, xcomm_tm_event_t, base);

        xcomm_timers_add(&loop->tm_evts, event);
        break;
    }
    default:
        break;
    }
}

void xcomm_eventloop_update(xcomm_eventloop_t* loop, xcomm_event_t* event) {
    platform_event_sqe_t sqe = {
        .op = event->operate, .handle = event->handle, .ud = event->userdata};
    platform_event_mod(loop->sq, &sqe);
}

void xcomm_eventloop_unregister(xcomm_eventloop_t* loop, xcomm_event_t* event) {
    
}

void xcomm_eventloop_run(xcomm_eventloop_t* loop) {
    platform_event_cqe_t cqes[PLATFORM_EVENT_CQE_NUM] = {0};
    while (loop->running) {
        int nevents =
            platform_event_wait(loop->sq, cqes, _timeout_update(loop));

        for (int i = 0; i < nevents; i++) {
            xcomm_event_t* event = cqes[i].ud;

            if (event && event->execute_cb) {
                event->execute_cb(event->context, cqes[i].op);
            }
        }
        if (!_timeout_update(loop)) {
            _timer_handle(loop);
        }
    }
}

void xcomm_eventloop_destroy(xcomm_eventloop_t* loop) {
}