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
#include "xcomm-sha256.h"

static void compare_hash(const uint8_t* digest, const char* expected_hex) {
    char actual_hex[65] = {0};

    for (int i = 0; i < 32; i++) {
        sprintf(actual_hex + i * 2, "%02x", digest[i]);
    }
    assert(strcmp(actual_hex, expected_hex) == 0);
}

static void test_empty_string(void) {
    xcomm_sha256_t ctx;
    uint8_t        digest[32];

    xcomm_sha256_init(&ctx);
    xcomm_sha256_update(&ctx, (uint8_t*)"", 0);
    xcomm_sha256_final(&ctx, digest);

    compare_hash(
        digest,
        "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855");
}

static void test_simple_string(void) {
    xcomm_sha256_t ctx;
    uint8_t        digest[32];

    xcomm_sha256_init(&ctx);
    xcomm_sha256_update(&ctx, (uint8_t*)"hello", 5);
    xcomm_sha256_final(&ctx, digest);

    compare_hash(
        digest,
        "2cf24dba5fb0a30e26e83b2ac5b9e29e1b161e5c1fa7425e73043362938b9824");
}

static void test_long_string(void) {
    xcomm_sha256_t ctx;
    uint8_t        digest[32];
    const char*    str = "This is a longer string that will test the SHA-256 "
                      "implementation more thoroughly.";

    xcomm_sha256_init(&ctx);
    xcomm_sha256_update(&ctx, (uint8_t*)str, strlen(str));
    xcomm_sha256_final(&ctx, digest);

    compare_hash(
        digest,
        "35b18b404ff1a1af93a5223ac9989c0b2e83387d08bbbb5772d0f75abf7d19b2");
}

static void test_chunked_update(void) {
    xcomm_sha256_t ctx;
    uint8_t        digest[32];

    xcomm_sha256_init(&ctx);
    xcomm_sha256_update(&ctx, (uint8_t*)"chunk1", 6);
    xcomm_sha256_update(&ctx, (uint8_t*)"chunk2", 6);
    xcomm_sha256_update(&ctx, (uint8_t*)"chunk3", 6);
    xcomm_sha256_final(&ctx, digest);

    compare_hash(
        digest,
        "bfe08b41e4577d49fb775d1dbc69d2db429bcec209e4637370cd88f2d6c96469");
}

static void test_binary_data(void) {
    xcomm_sha256_t ctx;
    uint8_t        digest[32];
    uint8_t binary_data[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07};

    xcomm_sha256_init(&ctx);
    xcomm_sha256_update(&ctx, binary_data, sizeof(binary_data));
    xcomm_sha256_final(&ctx, digest);

    compare_hash(
        digest,
        "8a851ff82ee7048ad09ec3847f1ddf44944104d2cbd17ef4e3db22c6785a0d45");
}

int main(void) {
    test_empty_string();
    test_simple_string();
    test_long_string();
    test_chunked_update();
    test_binary_data();
    return 0;
}