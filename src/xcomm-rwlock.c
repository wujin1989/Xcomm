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

#include "xcomm-rwlock.h"
#include <stdbool.h>

void xcomm_rwlock_init(xcomm_rwlock_t* restrict rwlock) {
    atomic_init(&rwlock->rd_cnt, 0);
    atomic_init(&rwlock->wr_waiters, 0);
    atomic_init(&rwlock->wr_locked, false);

    mtx_init(&rwlock->mutex, mtx_plain);
    cnd_init(&rwlock->rd_cond);
    cnd_init(&rwlock->wr_cond);
    return;
}

void xcomm_rwlock_destroy(xcomm_rwlock_t* restrict rwlock) {
    mtx_destroy(&rwlock->mutex);
    cnd_destroy(&rwlock->rd_cond);
    cnd_destroy(&rwlock->wr_cond);
}

void xcomm_rwlock_rdlock(xcomm_rwlock_t* restrict rwlock) {
    /* Fast Path */
    while (true) {
        bool has_writer =
            atomic_load_explicit(&rwlock->wr_locked, memory_order_acquire) ||
            atomic_load_explicit(&rwlock->wr_waiters, memory_order_acquire) > 0;

        if (has_writer) {
            break;
        }
        int expected =
            atomic_load_explicit(&rwlock->rd_cnt, memory_order_acquire);
        if (atomic_compare_exchange_weak_explicit(
                &rwlock->rd_cnt,
                &expected,
                expected + 1,
                memory_order_acq_rel,
                memory_order_relaxed)) {
            return;
        }
    }
    /* Slow Path */
    mtx_lock(&rwlock->mutex);
    while (atomic_load_explicit(&rwlock->wr_waiters, memory_order_acquire) ||
           atomic_load_explicit(&rwlock->wr_locked, memory_order_acquire)) {
        cnd_wait(&rwlock->rd_cond, &rwlock->mutex);
    }
    atomic_fetch_add_explicit(&rwlock->rd_cnt, 1, memory_order_release);
    mtx_unlock(&rwlock->mutex);
}

void xcomm_rwlock_wrlock(xcomm_rwlock_t* restrict rwlock) {
    atomic_fetch_add_explicit(&rwlock->wr_waiters, 1, memory_order_release);

    mtx_lock(&rwlock->mutex);
    while (atomic_load_explicit(&rwlock->rd_cnt, memory_order_acquire) ||
           atomic_load_explicit(&rwlock->wr_locked, memory_order_acquire)) {
        cnd_wait(&rwlock->wr_cond, &rwlock->mutex);
    }
    atomic_fetch_sub_explicit(&rwlock->wr_waiters, 1, memory_order_release);
    atomic_store_explicit(&rwlock->wr_locked, true, memory_order_release);
    mtx_unlock(&rwlock->mutex);
}

void xcomm_rwlock_rdunlock(xcomm_rwlock_t* restrict rwlock) {
    int  expected, desired;
    bool success = false;

    do {
        expected = atomic_load_explicit(&rwlock->rd_cnt, memory_order_acquire);
        if (expected <= 0) {
            abort();
        }
        desired = expected - 1;
        success = atomic_compare_exchange_weak_explicit(
            &rwlock->rd_cnt,
            &expected,
            desired,
            memory_order_acq_rel,
            memory_order_relaxed);
    } while (!success);

    if (expected == 1) {
        mtx_lock(&rwlock->mutex);
        cnd_signal(&rwlock->wr_cond);
        mtx_unlock(&rwlock->mutex);
    }
}

void xcomm_rwlock_wrunlock(xcomm_rwlock_t* restrict rwlock) {
    bool was_locked = atomic_exchange_explicit(
        &rwlock->wr_locked, false, memory_order_acq_rel);

    if (!was_locked) {
        abort();
    }
    mtx_lock(&rwlock->mutex);
    cnd_signal(&rwlock->wr_cond); /* Writer Priority */
    cnd_broadcast(&rwlock->rd_cond);
    mtx_unlock(&rwlock->mutex);
}