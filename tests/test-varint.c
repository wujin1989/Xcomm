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

#include "xcomm-varint.h"

static bool buffer_equal(const uint8_t* buf1, const uint8_t* buf2, int len) {
    return memcmp(buf1, buf2, len) == 0;
}

static void test_encode_decode(uint64_t value) {
    uint8_t buf[16];
    int     pos_encode = xcomm_varint_encode(value, buf);
    assert(pos_encode > 0 && pos_encode <= 10);

    int      pos_decode = 0;
    uint64_t decoded = xcomm_varint_decode(buf, &pos_decode);
    assert(decoded == value);
    assert(pos_decode == pos_encode);
}

static void
test_encoded_bytes(uint64_t value, const uint8_t* expected, int expected_len) {
    uint8_t buf[16];
    int     len = xcomm_varint_encode(value, buf);
    assert(len == expected_len);
    assert(buffer_equal(buf, expected, len));
}

static void test_varint(void) {
    test_encode_decode(0);
    test_encode_decode(1);
    test_encode_decode(127);
    test_encode_decode(128);
    test_encode_decode(300);
    test_encode_decode(0xFFFFFFFFFFFFFFFF);

    test_encoded_bytes(0, (uint8_t[]){0x00}, 1);
    test_encoded_bytes(127, (uint8_t[]){0x7F}, 1);
    test_encoded_bytes(128, (uint8_t[]){0x80, 0x01}, 2);
    test_encoded_bytes(300, (uint8_t[]){0xAC, 0x02}, 2);
    test_encoded_bytes(
        0xFFFFFFFFFFFFFFFF,
        (uint8_t[]){0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x01},
        10);

    uint8_t multi_buf[] = {0x00, 0x80, 0x01, 0xAC, 0x02}; // 0, 128, 300
    int     pos = 0;
    assert(xcomm_varint_decode(multi_buf, &pos) == 0);
    assert(pos == 1);
    assert(xcomm_varint_decode(multi_buf, &pos) == 128);
    assert(pos == 3);
    assert(xcomm_varint_decode(multi_buf, &pos) == 300);
    assert(pos == 5);
}

int main(void) {
    test_varint();
    return 0;
}