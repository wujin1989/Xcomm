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

void xcomm_eventloop_init(xcomm_eventloop_t* loop) {
    poller->pfd = platform_socket_pollfd_create();
    poller->tid = thrd_current();
    poller->active = true;
    poller->timermgr = cdk_timer_manager_create();

    cdk_list_init(&poller->evlist);
    cdk_list_init(&poller->chlist);

    mtx_init(&poller->evmtx, mtx_plain);
    platform_socket_socketpair(AF_INET, SOCK_STREAM, 0, poller->evfds);
    platform_socket_nonblock(poller->evfds[1]);
    platform_event_add(
        poller->pfd, poller->evfds[1], EVENT_RD, &poller->evfds[1]);

    platform_eventqueue_init(&loop->eventq);
    loop->tid = thrd_current();

}

void xcomm_eventloop_destroy(xcomm_eventloop_t* loop) {
}

void xcomm_eventloop_run(xcomm_eventloop_t* loop) {
}

void xcomm_eventloop_wakeup(xcomm_eventloop_t* loop) {
}

void xcomm_eventloop_register(
    xcomm_eventloop_t* loop, xcomm_event_t* event) {
}

void xcomm_eventloop_update(
    xcomm_eventloop_t* loop, xcomm_event_t* event) {
}

void xcomm_eventloop_unregister(xcomm_eventloop_t* loop, xcomm_event_t* event) {

}