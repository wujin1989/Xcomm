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
#include <stdbool.h>
#include <stdatomic.h>

#include "xcomm-rwlock.h"
#include "deprecated/c11-threads.h"

static atomic_int read_count = 0;
static int        shared_data = 0;
static atomic_int active_readers = 0;

static int reader_basic(void* arg) {
    xcomm_rwlock_t* rwlock = arg;

    xcomm_rwlock_rdlock(rwlock);
    assert(shared_data == 42);
    xcomm_rwlock_rdunlock(rwlock);
    return 0;
}

static int writer_basic(void* arg) {
    xcomm_rwlock_t* rwlock = arg;

    xcomm_rwlock_wrlock(rwlock);
    shared_data = 42;
    xcomm_rwlock_wrunlock(rwlock);
    return 0;
}

static void test_basic(void) {
    xcomm_rwlock_t rwlock;
    xcomm_rwlock_init(&rwlock);
    shared_data = 0;

    thrd_t writer, reader;
    thrd_create(&writer, writer_basic, &rwlock);
    thrd_join(writer, NULL);

    assert(shared_data == 42);

    thrd_create(&reader, reader_basic, &rwlock);
    thrd_join(reader, NULL);

    xcomm_rwlock_destroy(&rwlock);
}

static int writer_excl(void* arg) {
    xcomm_rwlock_t* rwlock = arg;

    xcomm_rwlock_wrlock(rwlock);
    shared_data++;
    thrd_sleep(&(struct timespec){.tv_sec = 0, .tv_nsec = 50000000}, NULL);
    xcomm_rwlock_wrunlock(rwlock);
    return 0;
}

static int reader_excl(void* arg) {
    xcomm_rwlock_t* rwlock = arg;

    xcomm_rwlock_rdlock(rwlock);
    assert(shared_data == 1);
    xcomm_rwlock_rdunlock(rwlock);
    return 0;
}

static void test_writer_exclusive(void) {
    xcomm_rwlock_t rwlock;
    xcomm_rwlock_init(&rwlock);
    shared_data = 0;

    thrd_t writer, reader;
    xcomm_rwlock_wrlock(&rwlock);

    thrd_create(&writer, writer_excl, &rwlock);
    thrd_create(&reader, reader_excl, &rwlock);

    thrd_sleep(&(struct timespec){.tv_sec = 0, .tv_nsec = 10000000}, NULL);

    xcomm_rwlock_wrunlock(&rwlock);

    thrd_join(writer, NULL);
    thrd_join(reader, NULL);

    assert(shared_data == 1);
    xcomm_rwlock_destroy(&rwlock);
}

static int concurrent_reader(void* arg) {
    xcomm_rwlock_t* rwlock = arg;

    xcomm_rwlock_rdlock(rwlock);
    atomic_fetch_add(&active_readers, 1);
    thrd_sleep(&(struct timespec){.tv_sec = 0, .tv_nsec = 100000000}, NULL);
    atomic_fetch_sub(&active_readers, 1);
    xcomm_rwlock_rdunlock(rwlock);
    return 0;
}

static void test_concurrent_readers(void) {
    xcomm_rwlock_t rwlock;
    xcomm_rwlock_init(&rwlock);
    atomic_store(&active_readers, 0);

    thrd_t readers[50];
    for (int i = 0; i < 50; i++) {
        thrd_create(&readers[i], concurrent_reader, &rwlock);
    }
    thrd_sleep(&(struct timespec){.tv_sec = 0, .tv_nsec = 50000000}, NULL);
    assert(atomic_load(&active_readers) == 50);

    for (int i = 0; i < 50; i++) {
        thrd_join(readers[i], NULL);
    }
    assert(atomic_load(&active_readers) == 0);
    xcomm_rwlock_destroy(&rwlock);
}

static int delayed_reader(void* arg) {
    xcomm_rwlock_t* rwlock = arg;

    xcomm_rwlock_rdlock(rwlock);
    atomic_fetch_add(&read_count, 1);
    xcomm_rwlock_rdunlock(rwlock);
    return 0;
}

static int priority_writer(void* arg) {
    xcomm_rwlock_t* rwlock = arg;

    xcomm_rwlock_wrlock(rwlock);
    shared_data = 999;
    xcomm_rwlock_wrunlock(rwlock);
    return 0;
}

static void test_writer_priority(void) {
    xcomm_rwlock_t rwlock;
    xcomm_rwlock_init(&rwlock);
    shared_data = 0;
    atomic_store(&read_count, 0);

    thrd_t writer;
    thrd_create(&writer, priority_writer, &rwlock);

    thrd_t readers[30];
    for (int i = 0; i < 30; i++) {
        thrd_create(&readers[i], delayed_reader, &rwlock);
    }
    thrd_join(writer, NULL);
    for (int i = 0; i < 30; i++) {
        thrd_join(readers[i], NULL);
    }
    assert(read_count == 30);
    xcomm_rwlock_destroy(&rwlock);
}

int main(void) {
    test_basic();
    test_writer_exclusive();
    test_concurrent_readers();
    test_writer_priority();
    return 0;
}