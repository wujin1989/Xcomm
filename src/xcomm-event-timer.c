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

static inline bool _event_timer_validate_timeout(xcomm_event_loop_t* loop) {
    if (xcomm_event_timer_empty(loop)) {
        return INT_MAX - 1;
    }
    xcomm_event_timer_t* timer = xcomm_event_timer_min(loop);
    if ((timer->event.tm.birth + timer->event.tm.expire) <=
        xcomm_utils_getnow(XCOMM_TIME_PRECISION_MSEC)) {
        return true;
    } else {
        return false;
    }
}

static void _event_timer_execute_cb(void* context, platform_event_op_t op) {
    (void)op;
    xcomm_event_timer_t* timer = (xcomm_event_timer_t*)context;

    if (_event_timer_validate_timeout(timer->loop)) {
        if (timer->routine) {
            timer->routine(timer->param);
        }
        if (timer->event.tm.repeat) {
            xcomm_event_timer_reset(timer->loop, timer, timer->event.tm.expire);
        } else {
            xcomm_event_timer_del(timer->loop, timer);
        }
    }
}

static void _event_timer_cleanup_cb(void* context, platform_event_op_t op) {
    (void)op;
    xcomm_event_timer_t* timer = (xcomm_event_timer_t*)context;

    if (timer->routine) {
        timer->routine(timer->param);
    }
    xcomm_event_timer_del(timer->loop, timer);
}

void xcomm_event_timer_del(
    xcomm_event_loop_t* loop, xcomm_event_timer_t* timer) {
    xcomm_event_loop_unregister(loop, &timer->event);
    free(timer);
}

void xcomm_event_timer_reset(
    xcomm_event_loop_t* loop, xcomm_event_timer_t* timer, uint64_t expire_ms) {
    xcomm_event_loop_unregister(loop, &timer->event);

    timer->event.tm.birth = xcomm_utils_getnow(XCOMM_TIME_PRECISION_MSEC);
    timer->event.tm.expire = expire_ms;

    xcomm_event_loop_register(loop, &timer->event);
}

bool xcomm_event_timer_empty(xcomm_event_loop_t* loop) {
    return xcomm_heap_empty(&loop->tm_ev_mgr);
}

xcomm_event_timer_t* xcomm_event_timer_min(xcomm_event_loop_t* loop) {
    if (xcomm_heap_empty(&loop->tm_ev_mgr)) {
        return NULL;
    }
    xcomm_heap_node_t* min = xcomm_heap_min(&loop->tm_ev_mgr);
    xcomm_event_t*     evt = xcomm_heap_data(min, xcomm_event_t, tm_node);

    return xcomm_event_timer_data(evt, xcomm_event_t, event);
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
    timer->loop    = loop;

    timer->event.type       = XCOMM_EVENT_TYPE_TM;
    timer->event.tm.birth   = xcomm_utils_getnow(XCOMM_TIME_PRECISION_MSEC);
    timer->event.tm.expire  = expire_ms;
    timer->event.tm.id      = loop->tm_ev_num;
    timer->event.tm.repeat  = repeat;
    timer->event.execute_cb = _event_timer_execute_cb;
    timer->event.cleanup_cb = _event_timer_cleanup_cb;
    timer->event.context    = timer;
    
    xcomm_event_loop_register(loop, &timer->event);
    return timer;
}
