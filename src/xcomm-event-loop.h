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
#include "xcomm-heap.h"
#include "xcomm-queue.h"

#include "platform/platform-types.h"

typedef struct xcomm_event_loop_s xcomm_event_loop_t;
typedef enum xcomm_event_type_e   xcomm_event_type_t;
typedef struct xcomm_event_s      xcomm_event_t;

struct xcomm_event_loop_s {
    bool                 running;
    thrd_t               tid;
    platform_poller_sq_t sq;
    platform_poller_fd_t wakefds[2];

    mtx_t                rt_ev_mtx;
    xcomm_queue_t        rt_ev_mgr;
    uint64_t             rt_ev_num;

    mtx_t                io_ev_mtx;
    xcomm_list_t         io_ev_mgr;
    uint64_t             io_ev_num;

    mtx_t                tm_ev_mtx;
    xcomm_heap_t         tm_ev_mgr;
    uint64_t             tm_ev_num;
};

enum xcomm_event_type_e {
    XCOMM_EVENT_TYPE_IO = 1, 
    XCOMM_EVENT_TYPE_RT = 2,
    XCOMM_EVENT_TYPE_TM = 3,
};

struct xcomm_event_s {
    xcomm_event_type_t  type;
    xcomm_event_loop_t* loop;

    struct {
        void (*execute_cb)(void* context);
        void (*cleanup_cb)(void* context);
    } rt;

    struct {
        void (*execute_cb)(void* context);
        void (*cleanup_cb)(void* context);
        int  (*calculate_timeout_cb)(void* context);
        int  (*compare_minheap_cb)(void* context1, void* context2);
    } tm;

    struct {
        void (*execute_cb)(void* context, platform_poller_op_t op);
        void (*cleanup_cb)(void* context);
        platform_poller_sqe_t sqe;
    } io;

    void* context;

    union {
        xcomm_queue_node_t rt_node;
        xcomm_heap_node_t  tm_node;
        xcomm_list_node_t  io_node;
    };
};

extern void xcomm_event_loop_init(xcomm_event_loop_t* loop);
extern void xcomm_event_loop_destroy(xcomm_event_loop_t* loop);
extern void xcomm_event_loop_stop(xcomm_event_loop_t* loop);
extern void xcomm_event_loop_run(xcomm_event_loop_t* loop);
extern void xcomm_event_loop_register(xcomm_event_loop_t* loop, xcomm_event_t* event);
extern void xcomm_event_loop_update(xcomm_event_loop_t* loop, xcomm_event_t* event);
extern void xcomm_event_loop_unregister(xcomm_event_loop_t* loop, xcomm_event_t* event);
