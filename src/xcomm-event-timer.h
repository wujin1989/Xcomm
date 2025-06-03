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
#include "xcomm-event-loop.h"
#include "deprecated/c11-threads.h"

typedef struct xcomm_event_timer_s xcomm_event_timer_t;

struct xcomm_event_timer_s {
    void (*routine)(void* param);
    xcomm_event_loop_t* loop;
    void*               param;
    uint64_t            id;
    uint64_t            birth;
    uint64_t            expire;
    bool                repeat;
    xcomm_event_t       event;
};

extern void xcomm_event_timer_del(xcomm_event_loop_t* loop, xcomm_event_timer_t* timer);
extern void xcomm_event_timer_reset(xcomm_event_loop_t* loop, xcomm_event_timer_t* timer, uint64_t expire_ms);
extern bool xcomm_event_timer_empty(xcomm_event_loop_t* loop);
extern xcomm_event_timer_t* xcomm_event_timer_min(xcomm_event_loop_t* loop);
extern xcomm_event_timer_t* xcomm_event_timer_add(xcomm_event_loop_t* loop, void (*routine)(void*), void* param, uint64_t expire_ms, bool repeat);