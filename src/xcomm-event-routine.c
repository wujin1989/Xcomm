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

#include "xcomm-event-routine.h"

typedef struct xcomm_event_routine_s xcomm_event_routine_t;

struct xcomm_event_routine_s {
    void (*routine)(void* param);
    void*         param;
    xcomm_event_t event;
};

static void _event_routine_execute_cb(void* context) {
    xcomm_event_routine_t* task = (xcomm_event_routine_t*)context;
    if (task->routine) {
        task->routine(task->param);
    }
    free(task);
}

void xcomm_event_routine_add(
    xcomm_event_loop_t* loop, void (*routine)(void*), void* param) {
    xcomm_event_routine_t* task = malloc(sizeof(xcomm_event_routine_t));
    if (!task) {
        return;
    }
    task->routine = routine;
    task->param   = param;

    task->event.type          = XCOMM_EVENT_TYPE_RT;
    task->event.loop          = loop;
    task->event.rt.execute_cb = _event_routine_execute_cb;
    task->event.context       = task;

    xcomm_event_loop_post(loop, &task->event);
}
