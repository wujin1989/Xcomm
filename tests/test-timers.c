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

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdatomic.h>

#include "xcomm-timer.h"
#include "xcomm-utils.h"

static atomic_int  trigger_count;
static atomic_bool poller_running;

static void test_callback(void* param) {
    atomic_fetch_add(&trigger_count, 1);
}

static int timer_poller(void* arg) {
    xcomm_timermgr_t* mgr = (xcomm_timermgr_t*)arg;
    poller_running = true;

    while (poller_running) {
        while (!xcomm_timer_empty(mgr)) {
            xcomm_timer_t* min = xcomm_timer_min(mgr);
            if (!min) {
                continue;
            }
            uint64_t now = xcomm_utils_getnow(XCOMM_TIME_PRECISION_MSEC);

            if (min->birth + min->expire > now) {
                thrd_yield();
                continue;
            }
            min->routine(min->param);
            if (min->repeat) {
                min->birth = now;
                xcomm_timer_reset(mgr, min, min->expire);
            } else {
                xcomm_timer_del(mgr, min);
            }
        }
        thrd_sleep(&(struct timespec){.tv_nsec = 10000000}, NULL);
    }
    return 0;
}

static void test_basic(void) {
    atomic_store(&trigger_count, 0);
    atomic_store(&poller_running, false);

    xcomm_timermgr_t mgr;
    xcomm_timer_manager_init(&mgr);
    thrd_t poller;
    thrd_create(&poller, timer_poller, &mgr);

    xcomm_timer_add(&mgr, test_callback, NULL, 200, false);

    thrd_sleep(&(struct timespec){.tv_nsec = 300000000}, NULL);
    assert(atomic_load(&trigger_count) == 1);

    poller_running = false;
    thrd_join(poller, NULL);
    xcomm_timer_manager_destroy(&mgr);
}

static void test_repeat(void) {
    atomic_store(&trigger_count, 0);
    atomic_store(&poller_running, false);

    xcomm_timermgr_t mgr;
    xcomm_timer_manager_init(&mgr);
    thrd_t poller;
    thrd_create(&poller, timer_poller, &mgr);

    xcomm_timer_t* t = xcomm_timer_add(&mgr, test_callback, NULL, 50, true);

    thrd_sleep(&(struct timespec){.tv_nsec = 220000000}, NULL);
    int count = atomic_load(&trigger_count);
    assert(count >= 4 && count <= 5);

    xcomm_timer_del(&mgr, t);

    poller_running = false;
    thrd_join(poller, NULL);
    xcomm_timer_manager_destroy(&mgr);
}

static void test_delete(void) {
    atomic_store(&trigger_count, 0);
    atomic_store(&poller_running, false);

    xcomm_timermgr_t mgr;
    xcomm_timer_manager_init(&mgr);
    thrd_t poller;
    thrd_create(&poller, timer_poller, &mgr);

    xcomm_timer_t* t = xcomm_timer_add(&mgr, test_callback, NULL, 100, false);
    xcomm_timer_del(&mgr, t);

    thrd_sleep(&(struct timespec){.tv_nsec = 150000000}, NULL);
    assert(atomic_load(&trigger_count) == 0);

    poller_running = false;
    thrd_join(poller, NULL);
    xcomm_timer_manager_destroy(&mgr);
}

static void test_reset(void) {
    atomic_store(&trigger_count, 0);
    atomic_store(&poller_running, false);

    xcomm_timermgr_t mgr;
    xcomm_timer_manager_init(&mgr);
    thrd_t poller;
    thrd_create(&poller, timer_poller, &mgr);

    xcomm_timer_t* t = xcomm_timer_add(&mgr, test_callback, NULL, 1000, false);
    assert(mgr.ntimers == 1);

    thrd_sleep(&(struct timespec){.tv_nsec = 100000000}, NULL);
    xcomm_timer_reset(&mgr, t, 100);
    assert(mgr.ntimers == 1);

    thrd_sleep(&(struct timespec){.tv_nsec = 150000000}, NULL);
    assert(atomic_load(&trigger_count) == 1);

    poller_running = false;
    thrd_join(poller, NULL);
    xcomm_timer_manager_destroy(&mgr);
}

int main(void) {
    test_basic();
    test_repeat();
    test_delete();
    test_reset();
    return 0;
}