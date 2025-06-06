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

void xcomm_utils_add_routine(void (*routine)(void* param), void* param) {
    xcomm_logi("%s enter.\n", __FUNCTION__);

    xcomm_event_routine_add(loop, routine, param);

    xcomm_logi("%s leave.\n", __FUNCTION__);
}

void xcomm_utils_del_timer(xcomm_utils_timer_t* timer) {
    xcomm_logi("%s enter.\n", __FUNCTION__);
    
    xcomm_event_timer_del(loop, timer->opaque);
    free(timer);

    xcomm_logi("%s leave.\n", __FUNCTION__);
}

xcomm_utils_timer_t* xcomm_utils_add_timer(
    void (*routine)(void* param),
    void*    param,
    uint64_t expire_ms,
    bool     repeat) {
    xcomm_logi("%s enter.\n", __FUNCTION__);

    xcomm_utils_timer_t* timer = malloc(sizeof(xcomm_utils_timer_t));
    if (!timer) {
        xcomm_loge("no memory.\n");
        return NULL;
    }
    timer->opaque = malloc(sizeof(xcomm_event_timer_t));
    if (!timer->opaque) {
        xcomm_loge("no memory.\n");
        free(timer);
        return NULL;
    }
    timer->opaque =
        xcomm_event_timer_add(loop, routine, param, expire_ms, repeat);

    xcomm_logi("%s leave.\n", __FUNCTION__);
    return timer;
}