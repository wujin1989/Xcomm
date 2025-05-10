/** Copyright (c) 2025, Archermind Tech. Co., Ltd. <marketing@archermind.com>
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

void platform_event_monitor_init(platform_event_monitor_t* monitor) {
    *monitor = epoll_create1(0);
}

void platform_event_monitor_destroy(platform_event_monitor_t* monitor) {
    epoll_close(*monitor);
}

void platform_event_add(
    platform_event_monitor_t* monitor,
    platform_sock_t           sock,
    int                       events,
    void*                     ud) {
    struct epoll_event ee = {0};

    if (events & PLATFORM_EVENT_RD_FLAG) {
        ee.events |= EPOLLIN;
    }
    if (events & PLATFORM_EVENT_WR_FLAG) {
        ee.events |= EPOLLOUT;
    }
    ee.data.ptr = ud;
    epoll_ctl(*monitor, EPOLL_CTL_ADD, sock, (struct epoll_event*)&ee);
}

void platform_event_mod(
    platform_event_monitor_t* monitor,
    platform_sock_t           sock,
    int                       events,
    void*                     ud) {
    struct epoll_event ee = {0};

    if (events & PLATFORM_EVENT_RD_FLAG) {
        ee.events |= EPOLLIN;
    }
    if (events & PLATFORM_EVENT_WR_FLAG) {
        ee.events |= EPOLLOUT;
    }
    ee.data.ptr = ud;
    epoll_ctl(*monitor, EPOLL_CTL_MOD, sock, (struct epoll_event*)&ee);
}

void platform_event_del(
    platform_event_monitor_t* monitor, platform_sock_t sock) {
    epoll_ctl(*monitor, EPOLL_CTL_DEL, sock, NULL);
}

int platform_event_wait(
    platform_event_monitor_t*      monitor,
    platform_event_notification_t* notifies,
    int                            timeout) {
    int                n;
    struct epoll_event epoll_events[PLATFORM_MAX_PROCESS_EVENTS] = {0};
    memset(
        notifies,
        0,
        sizeof(platform_event_notification_t) * PLATFORM_MAX_PROCESS_EVENTS);
    do {
        n = epoll_wait(
            *monitor, epoll_events, PLATFORM_MAX_PROCESS_EVENTS, timeout);
    } while (n == -1 && errno == EINTR);

    if (n < 0) {
        return 0;
    }
    for (int i = 0; i < n; i++) {
        notifies[i].ptr = epoll_events[i].data.ptr;
        if (epoll_events[i].events & (EPOLLIN | EPOLLHUP | EPOLLERR)) {
            notifies[i].events |= PLATFORM_EVENT_RD_FLAG;
        }
        if (epoll_events[i].events & (EPOLLOUT | EPOLLHUP | EPOLLERR)) {
            notifies[i].events |= PLATFORM_EVENT_WR_FLAG;
        }
    }
    return n;
}