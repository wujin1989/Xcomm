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
#include "xcomm-timers.h"
#include "xcomm-rbtree.h"

#include "platform/platform-types.h"

typedef struct xcomm_eventloop_s  xcomm_eventloop_t;
typedef enum xcomm_event_type_e   xcomm_event_type_t;
typedef struct xcomm_event_s      xcomm_event_t;
typedef struct xcomm_io_event_s   xcomm_io_event_t;
typedef struct xcomm_rt_event_s   xcomm_rt_event_t;
typedef struct xcomm_tm_event_s   xcomm_tm_event_t;

struct xcomm_eventloop_s {
    bool                running;
    thrd_t              tid;
    platform_event_sq_t sq;
    platform_sock_t     wakefds[2];
    mtx_t               rt_mtx;
    xcomm_list_t        rt_evts;
    xcomm_rbtree_t      io_evts;
    xcomm_timers_t      tm_evts;
};

enum xcomm_event_type_e {
    XCOMM_EVENT_TYPE_IO = 1, 
    XCOMM_EVENT_TYPE_RT = 2,
    XCOMM_EVENT_TYPE_TM = 3,
};

struct xcomm_event_s {
    xcomm_event_type_t type;

    void (*execute_cb)(void* context, platform_event_op_t op);
    void (*cleanup_cb)(void* context, platform_event_op_t op);
    void* context;
};

struct xcomm_rt_event_s {
    xcomm_event_t base;
};

struct xcomm_tm_event_s {
    xcomm_event_t base;
};

struct xcomm_io_event_s {
    xcomm_event_t        base;
    platform_event_sqe_t sqe;
    xcomm_rbtree_node_t  node;
};

extern void xcomm_eventloop_init(xcomm_eventloop_t* loop);
extern void xcomm_eventloop_destroy(xcomm_eventloop_t* loop);
extern void xcomm_eventloop_run(xcomm_eventloop_t* loop);
extern void xcomm_eventloop_register(xcomm_eventloop_t* loop, xcomm_event_t* event);
extern void xcomm_eventloop_update(xcomm_eventloop_t* loop, xcomm_event_t* event);
extern void xcomm_eventloop_unregister(xcomm_eventloop_t* loop, xcomm_event_t* event);
