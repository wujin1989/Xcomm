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

#include "xcomm-heap.h"
#include "deprecated/c11-threads.h"

typedef struct xcomm_timer_s  xcomm_timer_t;
typedef struct xcomm_timers_s xcomm_timers_t;

struct xcomm_timers_s {
    xcomm_heap_t heap;
    size_t       ntimers;
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

extern void xcomm_timers_init(xcomm_timers_t* timers);
extern void xcomm_timers_del(xcomm_timers_t* timers, xcomm_timer_t* timer);
extern void xcomm_timers_reset(xcomm_timers_t* timers, xcomm_timer_t* timer, size_t expire_ms);
extern bool xcomm_timers_empty(xcomm_timers_t* timers);
extern xcomm_timer_t* xcomm_timers_min(xcomm_timers_t* timers);
extern xcomm_timer_t* xcomm_timers_add(xcomm_timers_t* timers, void (*routine)(void*), void* param, size_t expire_ms, bool repeat);