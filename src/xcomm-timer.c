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

#include "xcomm-time.h"
#include "xcomm-timer.h"

static inline int _min_heapcmp(xcomm_heap_node_t* a, xcomm_heap_node_t* b) {
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

void xcomm_timer_manager_init(xcomm_timermgr_t* timermgr) {
    xcomm_heap_init(&timermgr->heap, _min_heapcmp);
    mtx_init(&timermgr->mtx, mtx_plain);
    timermgr->ntimers = 0;
}

void xcomm_timer_manager_destroy(xcomm_timermgr_t* timermgr) {
    timermgr->ntimers = 0;
    mtx_destroy(&timermgr->mtx);
}

void xcomm_timer_del(xcomm_timermgr_t* timermgr, xcomm_timer_t* timer) {
    mtx_lock(&timermgr->mtx);
    xcomm_heap_remove(&timermgr->heap, &timer->node);
    timermgr->ntimers--;
    mtx_unlock(&timermgr->mtx);

    free(timer);
}

void xcomm_timer_reset(
    xcomm_timermgr_t* timermgr, xcomm_timer_t* timer, size_t expire) {
    mtx_lock(&timermgr->mtx);
    xcomm_heap_remove(&timermgr->heap, &timer->node);
    timer->birth = xcomm_time_now();
    timer->expire = expire;
    xcomm_heap_insert(&timermgr->heap, &timer->node);
    mtx_unlock(&timermgr->mtx);
}

bool xcomm_timer_empty(xcomm_timermgr_t* timermgr) {
    mtx_lock(&timermgr->mtx);
    bool empty = xcomm_heap_empty(&timermgr->heap);
    mtx_unlock(&timermgr->mtx);
    return empty;
}

xcomm_timer_t* xcomm_timer_min(xcomm_timermgr_t* timermgr) {
    mtx_lock(&timermgr->mtx);
    xcomm_timer_t* timer =
        xcomm_heap_data(xcomm_heap_min(&timermgr->heap), xcomm_timer_t, node);
    mtx_unlock(&timermgr->mtx);
    return timer;
}

xcomm_timer_t* xcomm_timer_add(
    xcomm_timermgr_t* timermgr,
    void (*routine)(void*),
    void*             param,
    size_t            expire,
    bool              repeat) {
    xcomm_timer_t* timer = malloc(sizeof(xcomm_timer_t));
    if (timer) {
        mtx_lock(&timermgr->mtx);
        timer->param   = param;
        timer->birth   = xcomm_time_now();
        timer->expire  = expire;
        timer->repeat  = repeat;
        timer->routine = routine;
        timer->id      = timermgr->ntimers++;

        xcomm_heap_insert(&timermgr->heap, &timer->node);
        mtx_unlock(&timermgr->mtx);
    }
    return timer;
}
