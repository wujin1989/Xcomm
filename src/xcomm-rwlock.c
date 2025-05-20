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
#include "xcomm-rwlock.h"

void xcomm_rwlock_init(xcomm_rwlock_t* restrict rwlock) {
    atomic_init(&rwlock->rdcnt, 0);
    atomic_init(&rwlock->wrlock, false);
}

void xcomm_rwlock_rdlock(xcomm_rwlock_t* restrict rwlock) {
    while (true) {
        while (atomic_load(&rwlock->wrlock)) {
        }
        atomic_fetch_add(&rwlock->rdcnt, 1);
        if (!atomic_load(&rwlock->wrlock)) {
            break;
        }
        atomic_fetch_sub(&rwlock->rdcnt, 1);
    }
}

void xcomm_rwlock_wrlock(xcomm_rwlock_t* restrict rwlock) {
    while (atomic_exchange(&rwlock->wrlock, true)) {
    }
    while (atomic_load(&rwlock->rdcnt)) {
    }
}

void xcomm_rwlock_rdunlock(xcomm_rwlock_t* restrict rwlock) {
    atomic_fetch_sub(&rwlock->rdcnt, 1);
}

void xcomm_rwlock_wrunlock(xcomm_rwlock_t* restrict rwlock) {
    atomic_exchange(&rwlock->wrlock, false);
}