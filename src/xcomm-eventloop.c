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

#include "xcomm-eventloop.h"

#include "platform/platform-event.h"
#include "platform/platform-socket.h"

#define xcomm_eventloop_container_of(x, t, m) ((t*)((char*)(x)-offsetof(t, m)))

static void _io_event_cb(void* context) {
    xcomm_eventloop_t* loop = (xcomm_eventloop_t*)context;

    char buf;
    platform_socket_recv(loop->wkup[1], &buf, sizeof(buf));
}

void xcomm_eventloop_init(xcomm_eventloop_t* loop) {
    loop->running = true;
    loop->tid = thrd_current();
    
    mtx_init(&loop->rt_mtx, mtx_plain);
    
    xcomm_list_init(&loop->rt_evts);
    xcomm_rbtree_init(&loop->io_evts, xcomm_rbtree_keycmp_ptr);
    xcomm_timers_init(&loop->tm_evts);

    platform_event_init(&loop->sq);
    platform_socket_socketpair(AF_INET, SOCK_STREAM, 0, loop->wkup);
    platform_socket_enable_nonblocking(loop->wkup[1], true);

    xcomm_event_io_t* event = malloc(sizeof(xcomm_event_io_t));
    if (!event) {
        return;
    }
    platform_event_sqe_t sqe = {
        .op = PLATFORM_EVENT_RD_OP, 
        .fd = (platform_event_fd_t)loop->wkup[1],
        .ud = event
    };
    event->base.type = XCOMM_EVENT_TYPE_IO;
    event->base.context = loop;
    event->base.execute = _io_event_cb;
    event->base.cleanup = NULL;

    event->sqe = sqe;
    platform_event_add(loop->sq, &sqe);
}

void xcomm_eventloop_wakeup(xcomm_eventloop_t* loop) {
    char buf = 'W';
    platform_socket_send(loop->wkup[0], &buf, sizeof(buf));
}

void xcomm_eventloop_register(
    xcomm_eventloop_t* loop, xcomm_event_base_t* event) {
    switch (event->type) {
    case XCOMM_EVENT_TYPE_IO: {
        xcomm_event_io_t* io_evt =
            xcomm_eventloop_container_of(event, xcomm_event_io_t, base);

        platform_event_add(loop->sq, &io_evt->sqe);
        break;
    }
    case XCOMM_EVENT_TYPE_RT: {
        xcomm_event_rt_t* rt_evt =
            xcomm_eventloop_container_of(event, xcomm_event_rt_t, base);

        mtx_lock(&loop->rt_mtx);
        xcomm_list_insert_tail(&loop->rt_evts, event);
        mtx_unlock(&loop->rt_mtx);

        xcomm_eventloop_wakeup(loop);
        break;
    }
    case XCOMM_EVENT_TYPE_TM: {
        xcomm_event_tm_t* tm_evt =
            xcomm_eventloop_container_of(event, xcomm_event_tm_t, base);

        xcomm_timers_add(&loop->tm_evts, event);
        break;
    }
    default:
        break;
    }
}

void xcomm_eventloop_update(
    xcomm_eventloop_t* loop, xcomm_event_base_t* event) {
    platform_event_sqe_t sqe = {
        .op = event->operate, .handle = event->handle, .ud = event->userdata};
    platform_event_mod(loop->sq, &sqe);
}

void xcomm_eventloop_unregister(
    xcomm_eventloop_t* loop, xcomm_event_base_t* event) {
    
}

void xcomm_eventloop_run(xcomm_eventloop_t* loop) {
    platform_event_cqe_t cqes[PLATFORM_EVENT_CQE_NUM] = {0};
    while (loop->running) {
        int nevents =
            platform_event_wait(loop->sq, cqes, _timeout_update(poller));

        for (int i = 0; i < nevents; i++) {
            xcomm_event_t* event = cqes[i].ud;

            if (event && event->execute_cb) {
                event->execute(event->context);
            }
        }
        if (!_timeout_update(poller)) {
            _timer_handle(poller);
        }
    }
}

void xcomm_eventloop_destroy(xcomm_eventloop_t* loop) {
}