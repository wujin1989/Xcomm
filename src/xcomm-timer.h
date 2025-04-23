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

_Pragma("once")

#include "xcomm-heap.h"
#include "deprecated/c11-threads.h"

typedef struct xcomm_timer_s    xcomm_timer_t;
typedef struct xcomm_timermgr_s xcomm_timermgr_t;

struct xcomm_timermgr_s {
    xcomm_heap_t heap;
    size_t       ntimers;
    mtx_t        mtx;
};

struct xcomm_timer_s {
    void (*routine)(void* param);
    void*             param;
    size_t            birth;
    size_t            id;
    size_t            expire;
    bool              repeat;
    xcomm_heap_node_t node;
};

extern void xcomm_timer_manager_init(xcomm_timermgr_t* timermgr);
extern void xcomm_timer_manager_destroy(xcomm_timermgr_t* timermgr);
extern void xcomm_timer_del(xcomm_timermgr_t* timermgr, xcomm_timer_t* timer);
extern void xcomm_timer_reset(xcomm_timermgr_t* timermgr, xcomm_timer_t* timer, size_t expire);
extern bool xcomm_timer_empty(xcomm_timermgr_t* timermgr);
extern xcomm_timer_t* xcomm_timer_min(xcomm_timermgr_t* timermgr);
extern xcomm_timer_t* xcomm_timer_add(xcomm_timermgr_t* timermgr, void (*routine)(void*), void* param, size_t expire, bool repeat);