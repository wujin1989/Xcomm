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

#include "xcomm-bswap.h"
#include <assert.h>
#include <stdio.h>

static void test_bswap16(void) {
    int16_t m = 0x1234;
    int16_t n = 0x3412;
    assert(xcomm_bswap(m) == n);
}

static void test_bswap32(void) {
    int32_t m = 0x12345678;
    int32_t n = 0x78563412;
    assert(xcomm_bswap(m) == n);
}

static void test_bswap64(void) {
    int64_t m = 0x1234567890ABCDEF;
    int64_t n = 0xEFCDAB9078563412;
    assert(xcomm_bswap(m) == n);
}

static void test_bswap_float(void) {
    union {
        float    f32;
        uint32_t u32;
    } m, n;
    m.f32 = 12.34f;
    n.u32 = 0xA4704541;
    assert(xcomm_bswap(m.u32) == n.u32);
}

static void test_bswap_double(void) {
    union {
        double   f64;
        uint64_t u64;
    } m, n;
    m.f64 = 12.34;
    n.u64 = 0xAE47E17A14AE2840;
    assert(xcomm_bswap(m.u64) == n.u64);
}

int main(void) {
    test_bswap16();
    test_bswap32();
    test_bswap64();
    test_bswap_float();
    test_bswap_double();
    return 0;
}