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

void platform_eventqueue_destroy(platform_eventqueue_t* eventq) {
    close(*eventq);
}

#if defined(__linux__)
void platform_eventqueue_init(platform_eventqueue_t* eventq) {
    *eventq = epoll_create1(0);
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
#endif

#if defined(__APPLE__)

void platform_eventqueue_init(platform_eventqueue_t* eventq) {
    *eventq = kqueue();
}

void platform_eventqueue_add(
    platform_eventqueue_t* eventq,
    platform_sock_t        sock,
    int                    events,
    void*                  ud) {
    struct kevent ke = {0};

    if (events & PLATFORM_EVENT_RD_FLAG) {
        EV_SET(&ke, sock, EVFILT_READ, EV_ADD, 0, 0, ud);
        kevent(*eventq, &ke, 1, NULL, 0, NULL);
    }
    if (events & PLATFORM_EVENT_WR_FLAG) {
        EV_SET(&ke, sock, EVFILT_WRITE, EV_ADD, 0, 0, ud);
        kevent(*eventq, &ke, 1, NULL, 0, NULL);
    }
}

void platform_eventqueue_mod(
    platform_eventqueue_t* eventq,
    platform_sock_t        sock,
    int                    events,
    void*                  ud) {
    struct kevent ke = {0};

    if (events & PLATFORM_EVENT_RD_FLAG) {
        EV_SET(&ke, sock, EVFILT_READ, EV_ADD, 0, 0, ud);
        kevent(*eventq, &ke, 1, NULL, 0, NULL);
    }
    if (events & PLATFORM_EVENT_WR_FLAG) {
        EV_SET(&ke, sock, EVFILT_WRITE, EV_ADD, 0, 0, ud);
        kevent(*eventq, &ke, 1, NULL, 0, NULL);
    }
}

void platform_eventqueue_del(
    platform_eventqueue_t* eventq, platform_sock_t sock) {
    struct kevent ke = {0};

    EV_SET(&ke, sock, EVFILT_READ, EV_DELETE, 0, 0, NULL);
    kevent(*eventq, &ke, 1, NULL, 0, NULL);

    EV_SET(&ke, sock, EVFILT_WRITE, EV_DELETE, 0, 0, NULL);
    kevent(*eventq, &ke, 1, NULL, 0, NULL);
}

int platform_eventqueue_wait(
    platform_eventqueue_t* eventq, platform_event_cqe_t* cqe, int timeout) {
    struct kevent epoll_events[PLATFORM_EVENT_CQE_NUM];

    memset(cqe, 0, sizeof(platform_event_cqe_t) * PLATFORM_EVENT_CQE_NUM);
    struct timespec ts = {0, 0};
    ts.tv_sec  = (timeout / 1000UL);
    ts.tv_nsec = ((timeout % 1000UL) * 1000000UL);

    int n = kevent(*eventq, NULL, 0, epoll_events, PLATFORM_EVENT_CQE_NUM, &ts);
    /**
     * In systems utilizing the kqueue mechanism, read and write events are
     * handled independently, differing from the behavior of epoll. With epoll,
     * read and write events can be combined, allowing the use of bitwise
     * operations (such as the & operator) to check for specific event types on
     * a file descriptor. However, in kqueue, read and write events are treated
     * as entirely separate occurrences. This means that bitwise operations
     * cannot be simply applied to determine the event type. Consequently, when
     * processing events returned by kqueue, each event must be examined
     * individually to ascertain whether it is a read event or a write event,
     * and then the event handling logic should be updated accordingly.
     * This typically involves maintaining a separate state for each file
     * descriptor to track the event types that have occurred, rather than
     * employing bitwise operations within a single event mask.
     */
    for (int i = 0; i < n; i++) {
        cqe[i].ptr = epoll_events[i].udata;
        if (epoll_events[i].filter == EVFILT_READ) {
            cqe[i].events |= PLATFORM_EVENT_RD_FLAG;
        }
        if (epoll_events[i].filter == EVFILT_WRITE) {
            cqe[i].events |= PLATFORM_EVENT_WR_FLAG;
        }
    }
    return n;
}
#endif