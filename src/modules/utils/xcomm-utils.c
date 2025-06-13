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
#include "xcomm-logger.h"
#include "xcomm-event-timer.h"
#include "xcomm-event-routine.h"

typedef struct async_timer_context_s async_timer_context_t;

struct async_timer_context_s {
    void (*routine)(void* param);
    void*                param;
    uint64_t             expire_ms;
    bool                 repeat;
    xcomm_event_loop_t*  loop;
};

extern engine_t engine;

static void _async_timer_add(void* param) {
    async_timer_context_t* context = param;

    xcomm_event_timer_add(
        context->loop,
        context->routine,
        context->param,
        context->expire_ms,
        context->repeat);

    free(context);
}

void xcomm_utils_post_routine(void (*routine)(void* param), void* param) {
    xcomm_logi("%s enter.\n", __FUNCTION__);
    engine.roundrobin();
    xcomm_event_routine_add(loop, routine, param);

    xcomm_logi("%s leave.\n", __FUNCTION__);
}

void xcomm_utils_post_timer(
    void (*routine)(void* param),
    void*    param,
    uint64_t expire_ms,
    bool     repeat) {
    xcomm_logi("%s enter.\n", __FUNCTION__);

    async_timer_context_t* context = malloc(sizeof(async_timer_context_t));
    if (!context) {
        return;
    }
    context->routine   = routine;
    context->param     = param;
    context->expire_ms = expire_ms;
    context->repeat    = repeat;
    context->loop      = loop;

    xcomm_event_routine_add(loop, _async_timer_add, context);

    xcomm_logi("%s leave.\n", __FUNCTION__);
}