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

_Pragma("once")

#include "xcomm-queue.h"
#include "deprecated/c11-threads.h"

typedef struct xcomm_thrdpool_s xcomm_thrdpool_t;

struct xcomm_thrdpool_s {
    thrd_t*       thrds;
    size_t        thrdcnt;
    xcomm_queue_t queue;
    mtx_t         tmtx;
    mtx_t         qmtx;
    cnd_t         qcnd;
    bool          status;
};

extern void xcomm_thrdpool_init(xcomm_thrdpool_t* restrict pool, int nthrds);
extern void xcomm_thrdpool_post(xcomm_thrdpool_t* restrict pool, void (*routine)(void*), void* arg);
extern void xcomm_thrdpool_destroy(xcomm_thrdpool_t* restrict pool);