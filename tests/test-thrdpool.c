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
#include "xcomm-thrdpool.h"

typedef struct test_task_ctx_s {
    atomic_int* counter;
    xcomm_wg_t* wg;
    int         delay_ms;
} test_task_ctx_t;

static void test_task(void* arg) {
    test_task_ctx_t* ctx = (test_task_ctx_t*)arg;

    struct timespec ts = {
        .tv_sec = ctx->delay_ms / 1000,
        .tv_nsec = (ctx->delay_ms % 1000) * 1000000};
    thrd_sleep(&ts, NULL);

    atomic_fetch_add(ctx->counter, 1);
    xcomm_wg_done(ctx->wg);
}

static void test_task2(void* arg) {
    test_task_ctx_t* ctx = (test_task_ctx_t*)arg;
    xcomm_wg_done(ctx->wg);

    struct timespec ts = {
        .tv_sec = ctx->delay_ms / 1000,
        .tv_nsec = (ctx->delay_ms % 1000) * 1000000};
    thrd_sleep(&ts, NULL);

    atomic_fetch_add(ctx->counter, 1);
}

static void test_basic(void) {
    xcomm_thrdpool_t pool;
    xcomm_thrdpool_init(&pool, 2);

    atomic_int task_counter = 0;
    xcomm_wg_t wg;
    xcomm_wg_init(&wg);
    xcomm_wg_add(&wg, 1);

    test_task_ctx_t ctx = {
        .counter = &task_counter, .wg = &wg, .delay_ms = 100};

    xcomm_thrdpool_post(&pool, test_task, &ctx);
    xcomm_wg_wait(&wg);

    assert(atomic_load(&task_counter) == 1);

    xcomm_thrdpool_destroy(&pool);
    xcomm_wg_destroy(&wg);
}

static void test_concurrent(void) {
    xcomm_thrdpool_t pool;
    xcomm_thrdpool_init(&pool, 4);

    atomic_int task_counter = 0;
    xcomm_wg_t wg;
    xcomm_wg_init(&wg);
    xcomm_wg_add(&wg, 100);

    test_task_ctx_t* contexts = malloc(sizeof(test_task_ctx_t) * 100);
    for (int i = 0; i < 100; i++) {
        contexts[i] = (test_task_ctx_t){
            .counter = &task_counter, .wg = &wg, .delay_ms = 10};
        xcomm_thrdpool_post(&pool, test_task, &contexts[i]);
    }
    xcomm_wg_wait(&wg);
    assert(atomic_load(&task_counter) == 100);

    free(contexts);
    xcomm_thrdpool_destroy(&pool);
    xcomm_wg_destroy(&wg);
}

static void test_destroy(void) {
    xcomm_thrdpool_t pool;
    xcomm_thrdpool_init(&pool, 2);

    atomic_int task_counter = 0;
    xcomm_wg_t wg;
    xcomm_wg_init(&wg);
    xcomm_wg_add(&wg, 1);

    test_task_ctx_t ctx = {
        .counter = &task_counter, .wg = &wg, .delay_ms = 500};
    xcomm_thrdpool_post(&pool, test_task2, &ctx);
    xcomm_wg_wait(&wg);
    xcomm_thrdpool_destroy(&pool);

    assert(atomic_load(&task_counter) == 1);
    xcomm_wg_destroy(&wg);
}

int main(void) {
    test_basic();
    test_concurrent();
    test_destroy();
    return 0;
}