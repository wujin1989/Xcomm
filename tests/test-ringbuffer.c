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

void test_create_and_basic(void) {
    xcomm_ringbuf_t ring;
    xcomm_ringbuf_init(&ring, sizeof(int), 8);

    assert(xcomm_ringbuf_empty(&ring));
    assert(!xcomm_ringbuf_full(&ring));
    assert(xcomm_ringbuf_len(&ring) == 0);
    assert(xcomm_ringbuf_cap(&ring) == 8);
    assert(xcomm_ringbuf_avail(&ring) == 8);

    xcomm_ringbuf_destroy(&ring);
}

// 测试用例2：单条目读写
void test_single_read_write(void) {
    xcomm_ringbuf_t ring;
    xcomm_ringbuf_init(&ring, sizeof(int), 4);

    // 写入测试
    int      write_val = 42;
    uint32_t written = xcomm_ringbuf_write(&ring, &write_val, 1);
    assert(written == 1);
    assert(xcomm_ringbuf_len(&ring) == 1);
    assert(xcomm_ringbuf_avail(&ring) == 3);

    // 读取验证
    int      read_val;
    uint32_t read = xcomm_ringbuf_read(&ring, &read_val, 1);
    assert(read == 1);
    assert(read_val == 42);
    assert(xcomm_ringbuf_empty(&ring));

    xcomm_ringbuf_destroy(&ring);
}

// 测试用例3：满缓冲区处理
void test_full_condition(void) {
    xcomm_ringbuf_t ring;
    xcomm_ringbuf_init(&ring, sizeof(float), 4);

    // 填满缓冲区
    float    values[4] = {1.1f, 2.2f, 3.3f, 4.4f};
    uint32_t written = xcomm_ringbuf_write(&ring, values, 4);
    assert(written == 4);
    assert(xcomm_ringbuf_full(&ring));

    // 尝试超额写入
    float extra = 5.5f;
    written = xcomm_ringbuf_write(&ring, &extra, 1);
    assert(written == 0);
    assert(xcomm_ringbuf_full(&ring));

    xcomm_ringbuf_destroy(&ring);
}

// 测试用例4：空缓冲区处理
void test_empty_condition(void) {
    xcomm_ringbuf_t ring;
    xcomm_ringbuf_init(&ring, sizeof(char), 8);

    // 尝试读取空缓冲区
    char     buf[8];
    uint32_t read = xcomm_ringbuf_read(&ring, buf, 1);
    assert(read == 0);
    assert(xcomm_ringbuf_empty(&ring));

    xcomm_ringbuf_destroy(&ring);
}

// 测试用例5：循环覆盖写入
void test_wrap_around(void) {
    xcomm_ringbuf_t ring;
    xcomm_ringbuf_init(&ring, sizeof(int), 4); // 实际容量4条目

    // 写入3条目
    int data1[] = {1, 2, 3};
    xcomm_ringbuf_write(&ring, data1, 3);
    assert(xcomm_ringbuf_len(&ring) == 3);

    // 读取2条目
    int read_buf[3];
    xcomm_ringbuf_read(&ring, read_buf, 2);
    assert(memcmp(read_buf, data1, 2 * sizeof(int)) == 0);

    // 再写入3条目（应覆盖）
    int      data2[] = {4, 5, 6};
    uint32_t written = xcomm_ringbuf_write(&ring, data2, 3);
    assert(written == 3);
    assert(xcomm_ringbuf_len(&ring) == 4);

    // 验证内容
    int      expected[] = {3, 4, 5, 6};
    int      result[4];
    uint32_t read = xcomm_ringbuf_read(&ring, result, 4);
    assert(read == 4);
    assert(memcmp(expected, result, 4 * sizeof(int)) == 0);

    xcomm_ringbuf_destroy(&ring);
}

// 测试用例6：部分读写
void test_partial_io(void) {
    xcomm_ringbuf_t ring;
    xcomm_ringbuf_init(&ring, sizeof(double), 8);

    // 部分写入
    double   data[5] = {1.1, 2.2, 3.3, 4.4, 5.5};
    uint32_t written = xcomm_ringbuf_write(&ring, data, 5);
    assert(written == 5);
    assert(xcomm_ringbuf_avail(&ring) == 3);

    // 部分读取
    double   read_buf[3];
    uint32_t read = xcomm_ringbuf_read(&ring, read_buf, 3);
    assert(read == 3);
    assert(memcmp(data, read_buf, 3 * sizeof(double)) == 0);

    // 验证剩余数据
    assert(xcomm_ringbuf_len(&ring) == 2);

    xcomm_ringbuf_destroy(&ring);
}

int main(void) {
    test_create_and_basic();
    test_single_read_write();
    test_full_condition();
    test_empty_condition();
    test_wrap_around();
    test_partial_io();
    return 0;
}