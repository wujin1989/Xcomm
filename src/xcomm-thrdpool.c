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

#include "xcomm-thrdpool.h"

typedef struct thrdpool_job_s thrdpool_job_t;

struct thrdpool_job_s {
    void (*routine)(void*);
    void*              arg;
    xcomm_queue_node_t n;
};

static int _thrdpool_thrdfunc(void* arg) {
    xcomm_thrdpool_t* pool = arg;
    while (pool->status) {
        mtx_lock(&pool->qmtx);
        thrdpool_job_t* job;
        while (pool->status && xcomm_queue_empty(&pool->queue)) {
            cnd_wait(&pool->qcnd, &pool->qmtx);
        }
        xcomm_queue_node_t* node = xcomm_queue_dequeue(&pool->queue);
        if (node) {
            job = xcomm_queue_data(node, thrdpool_job_t, n);
            job->routine(job->arg);
            if (job) {
                free(job);
                job = NULL;
            }
        }
        mtx_unlock(&pool->qmtx);
    }
    return 0;
}

static void _thrdpool_thrd_create(xcomm_thrdpool_t* pool) {
    void* thrds;
    mtx_lock(&pool->tmtx);
    thrds = realloc(pool->thrds, (pool->thrdcnt + 1) * sizeof(thrd_t));
    if (thrds) {
        pool->thrds = thrds;
        thrd_create(pool->thrds + pool->thrdcnt, _thrdpool_thrdfunc, pool);
        pool->thrdcnt++;
    }
    mtx_unlock(&pool->tmtx);
}

void xcomm_thrdpool_init(xcomm_thrdpool_t* restrict pool, int nthrds) {
    xcomm_queue_init(&pool->queue);
    mtx_init(&pool->tmtx, mtx_plain);
    mtx_init(&pool->qmtx, mtx_plain);
    cnd_init(&pool->qcnd);

    pool->thrdcnt = 0;
    pool->status = true;
    pool->thrds = NULL;
    for (int i = 0; i < nthrds; i++) {
        _thrdpool_thrd_create(pool);
    }
}

void xcomm_thrdpool_post(
    xcomm_thrdpool_t* restrict pool, void (*routine)(void*), void* arg) {
    thrdpool_job_t* job = malloc(sizeof(thrdpool_job_t));
    if (job) {
        job->routine = routine;
        job->arg = arg;

        mtx_lock(&pool->qmtx);
        xcomm_queue_enqueue(&pool->queue, &job->n);
        cnd_signal(&pool->qcnd);
        mtx_unlock(&pool->qmtx);
    }
}

void xcomm_thrdpool_destroy(xcomm_thrdpool_t* restrict pool) {
    pool->status = false;
    cnd_broadcast(&pool->qcnd);
    for (int i = 0; i < pool->thrdcnt; i++) {
        thrd_join(pool->thrds[i], NULL);
    }
    mtx_destroy(&pool->qmtx);
    mtx_destroy(&pool->tmtx);
    cnd_destroy(&pool->qcnd);

    free(pool->thrds);
}