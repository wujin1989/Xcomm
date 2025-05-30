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

#include "xcomm-timer.h"
#include "xcomm-eventloop.h"

#include "platform/platform-event.h"
#include "platform/platform-socket.h"

static void _eventloop_post_routine(
    xcomm_eventloop_t* loop, void (*task)(void*), void* arg, bool totail) {
    cdk_async_event_t* async_event = malloc(sizeof(cdk_async_event_t));
    if (!async_event) {
        return;
    }
    async_event->task = task;
    async_event->arg = arg;

    mtx_lock(&poller->evmtx);
    if (totail) {
        cdk_list_insert_tail(&poller->evlist, &async_event->node);
    } else {
        cdk_list_insert_head(&poller->evlist, &async_event->node);
    }
    mtx_unlock(&poller->evmtx);
    poller_wakeup(poller);
}

void xcomm_eventloop_init(xcomm_eventloop_t* loop) {
    loop->running = true;
    loop->tid = thrd_current();
    
    mtx_init(&loop->rt_mtx, mtx_plain);
    
    xcomm_queue_init(&loop->rt_evts);
    xcomm_queue_init(&loop->ch_evts);
    xcomm_timer_manager_init(&loop->tm_mgr);

    platform_event_init(&loop->sq);
    platform_socket_socketpair(AF_INET, SOCK_STREAM, 0, loop->wakeup);
    platform_socket_enable_nonblocking(loop->wakeup[1], true);

    xcomm_event_t* event = malloc(sizeof(xcomm_event_t));
    if (!event) {
        return;
    }
    platform_event_sqe_t sqe = {
        .op = PLATFORM_EVENT_RD_OP, 
        .handle = (platform_event_handle_t)loop->wakeup[1],
        .ud = event
    };
    event->type = XCOMM_EVENT_CHANNEL;
    event->sqe = sqe;
    event->context = &loop->wakeup[1];
    event->execute_cb = ;
    event->cleanup_cb = ;

    platform_event_add(loop->sq, &sqe);
}

void xcomm_eventloop_wakeup(xcomm_eventloop_t* loop) {
}

void xcomm_eventloop_register(
    xcomm_eventloop_t* loop, xcomm_event_t* event) {
    switch (event->type) {
    case XCOMM_EVENT_ROUTINE:
    case XCOMM_EVENT_CHANNEL:
        platform_event_add(loop->sq, &event->sqe);
        break;
    default:
        break;
    }
}

void xcomm_eventloop_update(
    xcomm_eventloop_t* loop, xcomm_event_t* event) {
    platform_event_sqe_t sqe = {
        .op = event->operate, .handle = event->handle, .ud = event->userdata};
    platform_event_mod(loop->sq, &sqe);
}

void xcomm_eventloop_unregister(xcomm_eventloop_t* loop, xcomm_event_t* event) {
    platform_event_del(loop->sq, event->handle);
}

void xcomm_eventloop_run(xcomm_eventloop_t* loop) {
    platform_event_cqe_t cqes[PLATFORM_EVENT_CQE_NUM] = {0};
    while (loop->running) {
        int nevents =
            platform_event_wait(loop->sq, cqes, _timeout_update(poller));

        for (int i = 0; i < nevents; i++) {
            xcomm_event_t* event = cqes[i].ud;

            if (event && event->execute_cb) {
                event->execute_cb(event->context);
            }
        }
        if (!_timeout_update(poller)) {
            _timer_handle(poller);
        }
    }
}

void xcomm_eventloop_destroy(xcomm_eventloop_t* loop) {
}