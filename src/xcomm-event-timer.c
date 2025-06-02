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

#include "xcomm-utils.h"
#include "xcomm-event-timer.h"

void xcomm_event_timer_del(
    xcomm_event_loop_t* loop, xcomm_event_timer_t* timer) {
    xcomm_heap_remove(&loop->ev_tm_mgr, &timer->node);
    loop->ev_tm_num--;

    free(timer);
}

void xcomm_event_timer_reset(
    xcomm_event_loop_t* loop, xcomm_event_timer_t* timer, uint64_t expire_ms) {
    xcomm_heap_remove(&loop->ev_tm_mgr, &timer->node);

    timer->birth = xcomm_utils_getnow(XCOMM_TIME_PRECISION_MSEC);
    timer->expire = expire_ms;
    xcomm_heap_insert(&loop->ev_tm_mgr, &timer->node);
}

bool xcomm_event_timer_empty(xcomm_event_loop_t* loop) {
    return xcomm_heap_empty(&loop->ev_tm_mgr);
}

xcomm_event_timer_t* xcomm_event_timer_min(xcomm_event_loop_t* loop) {
    xcomm_heap_node_t* min = xcomm_heap_min(&loop->ev_tm_mgr);
    if (!min) {
        return NULL;
    }
    return xcomm_heap_data(min, xcomm_event_timer_t, node);
}

xcomm_event_timer_t* xcomm_event_timer_add(
    xcomm_event_loop_t* loop,
    void (*routine)(void*),
    void*               param,
    uint64_t            expire_ms,
    bool                repeat) {
    xcomm_event_timer_t* timer = malloc(sizeof(xcomm_event_timer_t));
    if (!timer) {
        return NULL;
    }
    timer->param   = param;
    timer->birth   = xcomm_utils_getnow(XCOMM_TIME_PRECISION_MSEC);
    timer->expire  = expire_ms;
    timer->repeat  = repeat;
    timer->routine = routine;
    
    xcomm_heap_insert(&loop->ev_tm_mgr, &timer->node);
    loop->ev_tm_num++;

    return timer;
}
