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

#include "platform/platform-event.h"
#include "wepoll/wepoll.h"

void platform_eventqueue_init(platform_eventqueue_t* eventq) {
    *eventq = epoll_create1(0);
}

void platform_eventqueue_destroy(platform_eventqueue_t* eventq) {
    epoll_close(*eventq);
}

void platform_eventqueue_add(
    platform_eventqueue_t* eventq,
    platform_sock_t        sock,
    int                    events,
    void*                  ud) {
    struct epoll_event ee = {0};

    if (events & PLATFORM_EVENT_RD_FLAG) {
        ee.events |= EPOLLIN;
    }
    if (events & PLATFORM_EVENT_WR_FLAG) {
        ee.events |= EPOLLOUT;
    }
    ee.data.ptr = ud;
    epoll_ctl(*eventq, EPOLL_CTL_ADD, sock, (struct epoll_event*)&ee);
}

void platform_eventqueue_mod(
    platform_eventqueue_t* eventq,
    platform_sock_t        sock,
    int                    events,
    void*                  ud) {
    struct epoll_event ee = {0};

    if (events & PLATFORM_EVENT_RD_FLAG) {
        ee.events |= EPOLLIN;
    }
    if (events & PLATFORM_EVENT_WR_FLAG) {
        ee.events |= EPOLLOUT;
    }
    ee.data.ptr = ud;
    epoll_ctl(*eventq, EPOLL_CTL_MOD, sock, (struct epoll_event*)&ee);
}

void platform_eventqueue_del(
    platform_eventqueue_t* eventq, platform_sock_t sock) {
    epoll_ctl(*eventq, EPOLL_CTL_DEL, sock, NULL);
}

int platform_eventqueue_wait(
    platform_eventqueue_t* eventq, platform_event_cqe_t* cqe, int timeout) {
    struct epoll_event epoll_events[PLATFORM_EVENT_CQE_NUM] = {0};
    memset(cqe, 0, sizeof(platform_event_cqe_t) * PLATFORM_EVENT_CQE_NUM);

    int n = 0;
    do {
        n = epoll_wait(*eventq, epoll_events, PLATFORM_EVENT_CQE_NUM, timeout);
    } while (n == -1 && errno == EINTR);

    if (n < 0) {
        return 0;
    }
    for (int i = 0; i < n; i++) {
        cqe[i].ptr = epoll_events[i].data.ptr;
        if (epoll_events[i].events & (EPOLLIN | EPOLLHUP | EPOLLERR)) {
            cqe[i].events |= PLATFORM_EVENT_RD_FLAG;
        }
        if (epoll_events[i].events & (EPOLLOUT | EPOLLHUP | EPOLLERR)) {
            cqe[i].events |= PLATFORM_EVENT_WR_FLAG;
        }
    }
    return n;
}