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

_Pragma("once")

#include "xcomm-list.h"
#include "xcomm-timer.h"
#include "platform/platform-types.h"

typedef struct xcomm_eventloop_s xcomm_eventloop_t;
typedef struct xcomm_event_s     xcomm_event_t;

typedef struct xcomm_eventloop_s {
    platform_eventqueue_t eventq;
    thrd_t                tid;
    platform_sock_t       evfds[2];
    xcomm_list_t          evlist;
    mtx_t                 evmtx;
    bool                  active;
    xcomm_list_t          chlist;
    xcomm_timermgr_t*     timermgr;
} xcomm_eventloop_t;

struct xcomm_event_s {
    platform_event_flag_t events;
    void (*callback)(void* context, platform_event_flag_t events);
    void* context;
};

extern void xcomm_eventloop_init(xcomm_eventloop_t* loop);
extern void xcomm_eventloop_destroy(xcomm_eventloop_t* loop);
extern void xcomm_eventloop_loop(xcomm_eventloop_t* loop);
extern void xcomm_eventloop_wakeup(xcomm_eventloop_t* loop);
extern void xcomm_eventloop_register(xcomm_eventloop_t* loop, platform_sock_t fd, xcomm_event_t* event);
extern void xcomm_eventloop_update(xcomm_eventloop_t* loop, platform_sock_t fd, xcomm_event_t* event);
extern void xcomm_eventloop_unregister(xcomm_eventloop_t* loop, platform_sock_t fd);
