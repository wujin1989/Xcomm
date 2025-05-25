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

#include "xcomm-thrdpool.h"

static atomic_int task_counter = 0;

static void test_task(void* arg) {
    int delay_ms = *(int*)arg;
    thrd_sleep(&(struct timespec){.tv_nsec = delay_ms * 1000000}, NULL);
    atomic_fetch_add(&task_counter, 1);
}

static void test_basic(void) {
    xcomm_thrdpool_t pool;
    xcomm_thrdpool_init(&pool, 2);

    int delay = 100;
    xcomm_thrdpool_post(&pool, test_task, &delay);
    thrd_sleep(&(struct timespec){.tv_sec = 1}, NULL);

    assert(atomic_load(&task_counter) == 1);
    xcomm_thrdpool_destroy(&pool);
}

static void test_concurrent(void) {
    xcomm_thrdpool_t pool;
    xcomm_thrdpool_init(&pool, 4);

    int delays[100];
    for (int i = 0; i < 100; i++) {
        delays[i] = 10;
        xcomm_thrdpool_post(&pool, test_task, &delays[i]);
    }
    thrd_sleep(&(struct timespec){.tv_sec = 2}, NULL);
    assert(atomic_load(&task_counter) == 100 + 1);

    xcomm_thrdpool_destroy(&pool);
}

static void test_destroy(void) {
    xcomm_thrdpool_t pool;
    xcomm_thrdpool_init(&pool, 2);

    int delay = 2000;
    xcomm_thrdpool_post(&pool, test_task, &delay);
    xcomm_thrdpool_destroy(&pool);

    assert(atomic_load(&task_counter) == 101);
}

int main(void) {
    test_basic();
    test_concurrent();
    test_destroy();
    return 0;
}