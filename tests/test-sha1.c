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
#include "xcomm-sha1.h"

static int compare_hex(const uint8_t* digest, const char* expected) {
    char actual[41] = {0};
    for (int i = 0; i < 20; i++) {
        sprintf(actual + i * 2, "%02x", digest[i]);
    }
    return strcmp(actual, expected) == 0;
}

static void test_empty_string(void) {
    xcomm_sha1_t ctx;
    uint8_t      digest[20];

    xcomm_sha1_init(&ctx);
    xcomm_sha1_update(&ctx, (uint8_t*)"", 0);
    xcomm_sha1_final(&ctx, digest);

    assert(compare_hex(digest, "da39a3ee5e6b4b0d3255bfef95601890afd80709"));
}

static void test_simple_string(void) {
    xcomm_sha1_t ctx;
    uint8_t      digest[20];

    xcomm_sha1_init(&ctx);
    xcomm_sha1_update(&ctx, (uint8_t*)"hello", 5);
    xcomm_sha1_final(&ctx, digest);

    assert(compare_hex(digest, "aaf4c61ddcc5e8a2dabede0f3b482cd9aea9434d"));
}

static void test_long_string(void) {
    xcomm_sha1_t ctx;
    uint8_t      digest[20];
    const char*  str = "This is a longer string that will test the SHA1 "
                      "implementation more thoroughly.";

    xcomm_sha1_init(&ctx);
    xcomm_sha1_update(&ctx, (uint8_t*)str, strlen(str));
    xcomm_sha1_final(&ctx, digest);

    assert(compare_hex(digest, "4218980779b7604e2c9d430027d8dc3ba508c04b"));
}

static void test_chunked_update(void) {
    xcomm_sha1_t ctx;
    uint8_t      digest[20];
    const char*  str = "chunk1chunk2chunk3";

    xcomm_sha1_init(&ctx);
    xcomm_sha1_update(&ctx, (uint8_t*)"chunk1", 6);
    xcomm_sha1_update(&ctx, (uint8_t*)"chunk2", 6);
    xcomm_sha1_update(&ctx, (uint8_t*)"chunk3", 6);
    xcomm_sha1_final(&ctx, digest);

    assert(compare_hex(digest, "022f039364f31a7faa21b2d49b6e962f8a3955b8"));
}

int main(void) {
    test_empty_string();
    test_simple_string();
    test_long_string();
    test_chunked_update();
    return 0;
}