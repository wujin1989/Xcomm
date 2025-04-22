/** Copyright (c) 2025, Archermind Tech. Co., Ltd. <marketing@archermind.com>
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

_Pragma("once")

#include "xcomm-types.h"

extern void     xcomm_ringbuf_create(xcomm_ringbuf_t* ring, uint32_t esize, uint32_t bufsize);
extern void     xcomm_ringbuf_destroy(xcomm_ringbuf_t* ring);
extern bool     xcomm_ringbuf_full(xcomm_ringbuf_t* ring);
extern bool     xcomm_ringbuf_empty(xcomm_ringbuf_t* ring);
extern uint32_t xcomm_ringbuf_len(xcomm_ringbuf_t* ring);
extern uint32_t xcomm_ringbuf_cap(xcomm_ringbuf_t* ring);
extern uint32_t xcomm_ringbuf_avail(xcomm_ringbuf_t* ring);
extern uint32_t xcomm_ringbuf_write(xcomm_ringbuf_t* ring, const void* buf, uint32_t entry_count);
extern uint32_t xcomm_ringbuf_read(xcomm_ringbuf_t* ring, void* buf, uint32_t entry_count);