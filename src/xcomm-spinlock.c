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

#include <stdbool.h>
#include "xcomm-spinlock.h"
#include "deprecated/c11-threads.h"

void xcomm_spinlock_init(xcomm_spinlock_t* restrict lock) {
    atomic_init(&lock->locked, false);
}

void xcomm_spinlock_destroy(xcomm_spinlock_t* restrict lock) {
    (void)lock;
}

void xcomm_spinlock_lock(xcomm_spinlock_t* restrict lock) {
    while (
        atomic_exchange_explicit(&lock->locked, true, memory_order_acquire)) {
        thrd_yield();
    }
}

void xcomm_spinlock_unlock(xcomm_spinlock_t* restrict lock) {
    atomic_exchange_explicit(&lock->locked, false, memory_order_release);
}