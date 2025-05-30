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
#include "xcomm-timers.h"

static inline int _minheap_cmp(xcomm_heap_node_t* a, xcomm_heap_node_t* b) {
    xcomm_timer_t* ta = xcomm_heap_data(a, xcomm_timer_t, node);
    xcomm_timer_t* tb = xcomm_heap_data(b, xcomm_timer_t, node);

    if ((ta->birth + ta->expire) < (tb->birth + tb->expire)) {
        return 1;
    }
    if ((ta->birth + ta->expire) == (tb->birth + tb->expire)) {
        if ((ta->id < tb->id)) {
            return 1;
        }
    }
    return 0;
}

void xcomm_timers_init(xcomm_timers_t* timers) {
    xcomm_heap_init(&timers->heap, _minheap_cmp);
    timers->ntimers = 0;
}

void xcomm_timers_del(xcomm_timers_t* timers, xcomm_timer_t* timer) {
    xcomm_heap_remove(&timers->heap, &timer->node);
    timers->ntimers--;
    free(timer);
}

void xcomm_timers_reset(
    xcomm_timers_t* timers, xcomm_timer_t* timer, size_t expire_ms) {
    xcomm_heap_remove(&timers->heap, &timer->node);

    timer->birth = xcomm_utils_getnow(XCOMM_TIME_PRECISION_MSEC);
    timer->expire = expire_ms;
    xcomm_heap_insert(&timers->heap, &timer->node);
}

bool xcomm_timers_empty(xcomm_timers_t* timers) {
    return xcomm_heap_empty(&timers->heap);
}

xcomm_timer_t* xcomm_timers_min(xcomm_timers_t* timers) {
    xcomm_heap_node_t* min = xcomm_heap_min(&timers->heap);
    if (!min) {
        return NULL;
    }
    return xcomm_heap_data(min, xcomm_timer_t, node);
}

xcomm_timer_t* xcomm_timers_add(
    xcomm_timers_t* timers,
    void (*routine)(void*),
    void*           param,
    size_t          expire_ms,
    bool            repeat) {
    xcomm_timer_t* timer = malloc(sizeof(xcomm_timer_t));
    if (!timer) {
        return NULL;
    }
    timer->id = timers->ntimers++;
    timer->param = param;
    timer->birth = xcomm_utils_getnow(XCOMM_TIME_PRECISION_MSEC);
    timer->expire = expire_ms;
    timer->repeat = repeat;
    timer->routine = routine;
    
    xcomm_heap_insert(&timers->heap, &timer->node);
    return timer;
}
