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
#include "xcomm-ringbuffer.h"

static void test_create_and_basic(void) {
    xcomm_ringbuf_t ring;
    xcomm_ringbuf_init(&ring, sizeof(int), 8);

    assert(xcomm_ringbuf_empty(&ring));
    assert(!xcomm_ringbuf_full(&ring));
    assert(xcomm_ringbuf_len(&ring) == 0);
    assert(xcomm_ringbuf_cap(&ring) == 2);
    assert(xcomm_ringbuf_avail(&ring) == 2);

    xcomm_ringbuf_destroy(&ring);
}

static void test_single_read_write(void) {
    xcomm_ringbuf_t ring;
    xcomm_ringbuf_init(&ring, sizeof(int), 4);

    int      write_val = 42;
    uint32_t written = xcomm_ringbuf_write(&ring, &write_val, 1);
    assert(written == 1);
    assert(xcomm_ringbuf_len(&ring) == 1);
    assert(xcomm_ringbuf_avail(&ring) == 0);

    int      read_val;
    uint32_t read = xcomm_ringbuf_read(&ring, &read_val, 1);
    assert(read == 1);
    assert(read_val == 42);
    assert(xcomm_ringbuf_empty(&ring));

    xcomm_ringbuf_destroy(&ring);
}

static void test_full_condition(void) {
    xcomm_ringbuf_t ring;
    xcomm_ringbuf_init(&ring, sizeof(float), 16);

    float    values[4] = {1.1f, 2.2f, 3.3f, 4.4f};
    uint32_t written = xcomm_ringbuf_write(&ring, values, 4);
    assert(written == 4);
    assert(xcomm_ringbuf_full(&ring));

    float extra = 5.5f;
    written = xcomm_ringbuf_write(&ring, &extra, 1);
    assert(written == 0);
    assert(xcomm_ringbuf_full(&ring));

    xcomm_ringbuf_destroy(&ring);
}

static void test_empty_condition(void) {
    xcomm_ringbuf_t ring;
    xcomm_ringbuf_init(&ring, sizeof(char), 8);

    char     buf[8];
    uint32_t read = xcomm_ringbuf_read(&ring, buf, 1);
    assert(read == 0);
    assert(xcomm_ringbuf_empty(&ring));

    xcomm_ringbuf_destroy(&ring);
}

static void test_partial_io(void) {
    xcomm_ringbuf_t ring;
    xcomm_ringbuf_init(&ring, sizeof(double), 64);

    double   data[5] = {1.1, 2.2, 3.3, 4.4, 5.5};
    uint32_t written = xcomm_ringbuf_write(&ring, data, 5);
    assert(written == 5);
    assert(xcomm_ringbuf_avail(&ring) == 3);

    double   read_buf[3];
    uint32_t read = xcomm_ringbuf_read(&ring, read_buf, 3);
    assert(read == 3);
    assert(memcmp(data, read_buf, 3 * sizeof(double)) == 0);
    assert(xcomm_ringbuf_len(&ring) == 2);

    xcomm_ringbuf_destroy(&ring);
}

int main(void) {
    test_create_and_basic();
    test_single_read_write();
    test_full_condition();
    test_empty_condition();
    test_partial_io();
    return 0;
}