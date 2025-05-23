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
#include "xcomm-base64.h"

void test_base64_encode(void) {
    size_t      dstlen = 0;
    uint8_t     dst[256] = {0};
    uint8_t     src[] = "Hello, World!";
    const char* expected = "SGVsbG8sIFdvcmxkIQ==";

    xcomm_base64_encode(src, strlen((char*)src), dst, &dstlen);
    assert(strcmp((char*)dst, expected) == 0);
    assert(dstlen == strlen(expected));
}

void test_base64_decode(void) {
    size_t      dstlen = 0;
    uint8_t     dst[256] = {0};
    const char* src = "SGVsbG8sIFdvcmxkIQ==";
    const char* expected = "Hello, World!";

    xcomm_base64_decode((uint8_t*)src, strlen(src), dst, &dstlen);
    assert(strcmp((char*)dst, expected) == 0);
    assert(dstlen == strlen(expected));
}

int main(void) {
    test_base64_encode();
    test_base64_decode();
    return 0;
}