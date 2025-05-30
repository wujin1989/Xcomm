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

void platform_event_init(platform_event_sq_t* sq) {
    *sq = epoll_create1(0);
}

void platform_event_destroy(platform_event_sq_t* sq) {
    epoll_close(*sq);
}

void platform_event_add(platform_event_sq_t* sq, platform_event_sqe_t* sqe) {
    struct epoll_event ee = {0};

    if (sqe->op & PLATFORM_EVENT_RD_OP) {
        ee.events |= EPOLLIN;
    }
    if (sqe->op & PLATFORM_EVENT_WR_OP) {
        ee.events |= EPOLLOUT;
    }
    ee.data.ptr = sqe->ud;
    epoll_ctl(*sq, EPOLL_CTL_ADD, sqe->handle, (struct epoll_event*)&ee);
}

void platform_event_mod(platform_event_sq_t* sq, platform_event_sqe_t* sqe) {
    struct epoll_event ee = {0};

    if (sqe->op & PLATFORM_EVENT_RD_OP) {
        ee.events |= EPOLLIN;
    }
    if (sqe->op & PLATFORM_EVENT_WR_OP) {
        ee.events |= EPOLLOUT;
    }
    ee.data.ptr = sqe->ud;
    epoll_ctl(*sq, EPOLL_CTL_MOD, sqe->handle, (struct epoll_event*)&ee);
}

void platform_event_del(platform_event_sq_t* sq, platform_event_sqe_t* sqe) {
    epoll_ctl(*sq, EPOLL_CTL_DEL, &sqe->handle, NULL);
}

int platform_event_wait(
    platform_event_sq_t* sq, platform_event_cqe_t* cqe, int timeout) {
    struct epoll_event events[PLATFORM_EVENT_CQE_NUM] = {0};
    memset(cqe, 0, sizeof(platform_event_cqe_t) * PLATFORM_EVENT_CQE_NUM);

    int n = 0;
    do {
        n = epoll_wait(*sq, events, PLATFORM_EVENT_CQE_NUM, timeout);
    } while (n == -1 && errno == EINTR);

    if (n < 0) {
        return 0;
    }
    for (int i = 0; i < n; i++) {
        cqe[i].ud = events[i].data.ptr;
        if (events[i].events & (EPOLLIN | EPOLLHUP | EPOLLERR)) {
            cqe[i].op |= PLATFORM_EVENT_RD_OP;
        }
        if (events[i].events & (EPOLLOUT | EPOLLHUP | EPOLLERR)) {
            cqe[i].op |= PLATFORM_EVENT_WR_OP;
        }
    }
    return n;
}