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

#define xcomm_event_timer_data(x, t, m) ((t *)((char *)(x) - offsetof(t, m)))

static void _event_timer_execute_cb(void* context) {
    xcomm_event_timer_t* timer = (xcomm_event_timer_t*)context;

    if (timer->routine) {
        timer->routine(timer->param);
    }
    if (timer->repeat) {
        xcomm_event_timer_reset(timer->event.loop, timer, timer->expire);
    } else {
        xcomm_event_timer_del(timer->event.loop, timer);
    }
}

static void _event_timer_cleanup_cb(void* context) {
    xcomm_event_timer_t* timer = (xcomm_event_timer_t*)context;

    if (timer->routine) {
        timer->routine(timer->param);
    }
    xcomm_event_timer_del(timer->event.loop, timer);
}

static int _event_timer_calculate_timeout_cb(void* context) {
    xcomm_event_timer_t* timer = (xcomm_event_timer_t*)context;

    uint64_t now = xcomm_utils_getnow(XCOMM_TIME_PRECISION_MSEC);
    if (timer->birth + timer->expire <= now) {
        return 0;
    }
    return (int)(timer->birth + timer->expire - now);
}

static int _event_timer_compare_minheap_cb(void* context1, void* context2) {
    xcomm_event_timer_t* timer1 = (xcomm_event_timer_t*)context1;
    xcomm_event_timer_t* timer2 = (xcomm_event_timer_t*)context2;

    if ((timer1->birth + timer1->expire) < (timer2->birth + timer2->expire)) {
        return 1;
    }
    if ((timer1->birth + timer1->expire) > (timer2->birth + timer2->expire)) {
        return 0;
    }
    uint64_t id1 = atomic_load_explicit(
        &timer1->event.loop->tm_ev_id, memory_order_relaxed);

    uint64_t id2 = atomic_load_explicit(
        &timer2->event.loop->tm_ev_id, memory_order_relaxed);

    return id1 < id2 ? 1 : 0;
}

void xcomm_event_timer_del(
    xcomm_event_loop_t* loop, xcomm_event_timer_t* timer) {
    xcomm_event_loop_unregister(loop, &timer->event);
    free(timer);
}

void xcomm_event_timer_reset(
    xcomm_event_loop_t* loop, xcomm_event_timer_t* timer, uint64_t expire_ms) {
    xcomm_event_loop_unregister(loop, &timer->event);

    timer->birth = xcomm_utils_getnow(XCOMM_TIME_PRECISION_MSEC);
    timer->expire = expire_ms;

    xcomm_event_loop_register(loop, &timer->event);
}

bool xcomm_event_timer_empty(xcomm_event_loop_t* loop) {
    return xcomm_heap_empty(&loop->tm_ev_mgr);
}

xcomm_event_timer_t* xcomm_event_timer_min(xcomm_event_loop_t* loop) {
    if (xcomm_heap_empty(&loop->tm_ev_mgr)) {
        return NULL;
    }
    xcomm_event_t* base = xcomm_heap_data(
        xcomm_heap_min(&loop->tm_ev_mgr), xcomm_event_t, tm_node);

    return xcomm_event_timer_data(base, xcomm_event_timer_t, event);
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
    timer->routine = routine;
    timer->param   = param;
    timer->id      = atomic_fetch_add_explicit(&loop->tm_ev_id, 1, memory_order_relaxed);
    timer->birth   = xcomm_utils_getnow(XCOMM_TIME_PRECISION_MSEC);
    timer->expire  = expire_ms;
    timer->repeat  = repeat;

    timer->event.type    = XCOMM_EVENT_TYPE_TM;
    timer->event.loop    = loop;
    timer->event.context = timer;

    timer->event.tm.execute_cb           = _event_timer_execute_cb;
    timer->event.tm.cleanup_cb           = _event_timer_cleanup_cb;
    timer->event.tm.calculate_timeout_cb = _event_timer_calculate_timeout_cb;
    timer->event.tm.compare_minheap_cb   = _event_timer_compare_minheap_cb;
    
    
    xcomm_event_loop_register(loop, &timer->event);
    return timer;
}
