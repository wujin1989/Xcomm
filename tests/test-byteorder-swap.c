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
#include <assert.h>
#include "xcomm-bswap.h"

int main(void) {
    int16_t m16 = 0x1234;
    int16_t n16 = 0x3412;
    assert(xcomm_bswap(m16) == n16);

    int32_t m32 = 0x12345678;
    int32_t n32 = 0x78563412;
    assert(xcomm_bswap(m32) == n32);

    int64_t m64 = 0x1234567890ABCDEF;
    int64_t n64 = 0xEFCDAB9078563412;
    assert(xcomm_bswap(m64) == n64);

    union {
        float    f32;
        uint32_t u32;
    } mf, nf;
    mf.f32 = 12.34f;
    nf.u32 = 0xA4704541;
    assert(xcomm_bswap(mf.u32) == nf.u32);

    union {
        double   f64;
        uint64_t u64;
    } md, nd;
    md.f64 = 12.34;
    nd.u64 = 0xAE47E17A14AE2840;
    assert(xcomm_bswap(md.u64) == nd.u64);

    return 0;
}