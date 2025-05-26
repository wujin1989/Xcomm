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

#include "xcomm-wg.h"

static atomic_int wait_test_flag = 0;

static void test_init_destroy(void) {
    xcomm_wg_t wg;
    xcomm_wg_init(&wg);
    xcomm_wg_destroy(&wg);
}

static void test_single_thread(void) {
    xcomm_wg_t wg;
    xcomm_wg_init(&wg);

    xcomm_wg_add(&wg, 3);
    xcomm_wg_done(&wg);
    xcomm_wg_done(&wg);
    xcomm_wg_done(&wg);
    xcomm_wg_wait(&wg);

    xcomm_wg_destroy(&wg);
}

static int worker_done(void* arg) {
    xcomm_wg_t* wg = arg;
    xcomm_wg_done(wg);
    return 0;
}

static void test_multiple_threads(void) {
    xcomm_wg_t wg;
    xcomm_wg_init(&wg);

    xcomm_wg_add(&wg, 5);

    thrd_t threads[5];
    for (int i = 0; i < 5; i++) {
        thrd_create(&threads[i], worker_done, &wg);
    }
    xcomm_wg_wait(&wg);

    for (int i = 0; i < 5; i++) {
        thrd_join(threads[i], NULL);
    }
    xcomm_wg_destroy(&wg);
}

static void test_add_negative(void) {
    xcomm_wg_t wg;
    xcomm_wg_init(&wg);

    xcomm_wg_add(&wg, 10);
    xcomm_wg_add(&wg, -5);
    xcomm_wg_add(&wg, -5);

    xcomm_wg_wait(&wg);

    xcomm_wg_destroy(&wg);
}

static int worker_add_done(void* arg) {
    xcomm_wg_t* wg = arg;
    xcomm_wg_add(wg, 1);
    struct timespec delay = {.tv_nsec = 1000000};
    thrd_sleep(&delay, NULL);
    xcomm_wg_done(wg);
    return 0;
}

static void test_concurrent_operations(void) {
    xcomm_wg_t wg;
    xcomm_wg_init(&wg);

    thrd_t    threads[10];
    for (int i = 0; i < 10; i++) {
        thrd_create(&threads[i], worker_add_done, &wg);
    }
    xcomm_wg_wait(&wg);

    for (int i = 0; i < 10; i++) {
        thrd_join(threads[i], NULL);
    }
    xcomm_wg_destroy(&wg);
}

static int worker_done_with_flag(void* arg) {
    xcomm_wg_t*     wg = arg;
    struct timespec delay = {.tv_nsec = 50000000};
    thrd_sleep(&delay, NULL);
    wait_test_flag = 1;
    xcomm_wg_done(wg);
    return 0;
}

static void test_wait_blocks(void) {
    xcomm_wg_t wg;
    xcomm_wg_init(&wg);

    xcomm_wg_add(&wg, 1);
    thrd_t thread;
    thrd_create(&thread, worker_done_with_flag, &wg);
    assert(wait_test_flag == 0);
    xcomm_wg_wait(&wg);
    assert(wait_test_flag == 1);

    thrd_join(thread, NULL);
    xcomm_wg_destroy(&wg);
}

int main(void) {
    test_init_destroy();
    test_single_thread();
    test_multiple_threads();
    test_add_negative();
    test_concurrent_operations();
    test_wait_blocks();
    return 0;
}