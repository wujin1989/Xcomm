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

#include <stdatomic.h>

#include "xcomm.h"
#include "xcomm-wg.h"
#include "xcomm-event-loop.h"
#include "deprecated/c11-threads.h"

#include "platform/platform-socket.h"

typedef struct engine_s        engine_t;
typedef struct engine_worker_s engine_worker_t;

struct engine_worker_s {
    xcomm_event_loop_t looper;
    atomic_int         refcnt;
    xcomm_list_node_t  node;
};

struct engine_s {
    atomic_flag  initialized;
    xcomm_list_t workers;
    mtx_t        mutex;
    cnd_t        cond;
    xcomm_wg_t   waitgroup;
    engine_worker_t* (*roundrobin)(void);
};

engine_t engine = {
    .initialized = ATOMIC_FLAG_INIT,
    .workers     = {0},
    .waitgroup   = {0},
    .roundrobin  = NULL
};

static void _engine_worker_ref(engine_worker_t* worker) {
    atomic_fetch_add(&worker->refcnt, 1);
}

static void _engine_worker_unref(engine_worker_t* worker) {
    if (atomic_fetch_sub(&worker->refcnt, 1) == 1) {
        free(worker);
    }
}

static engine_worker_t* _engine_roundrobin(void) {
    mtx_lock(&engine.mutex);

    while (xcomm_list_empty(&engine.workers)) {
        cnd_wait(&engine.cond, &engine.mutex);
    }
    xcomm_list_node_t* head = xcomm_list_head(&engine.workers);
    engine_worker_t* worker = xcomm_list_data(head, engine_worker_t, node);
    
    _engine_worker_ref(worker);

    xcomm_list_remove(head);
    xcomm_list_insert_tail(&engine.workers, head);

    mtx_unlock(&engine.mutex);

    return worker;
}

static engine_worker_t* _engine_create_worker(void) {
    engine_worker_t* worker = calloc(1, sizeof(engine_worker_t));
    if (!worker) {
        return NULL;
    }
    atomic_init(&worker->refcnt, 0);
    _engine_worker_ref(worker);

    xcomm_event_loop_init(&worker->looper);

    mtx_lock(&engine.mutex);
    xcomm_list_insert_tail(&engine.workers, &worker->node);
    cnd_signal(&engine.cond);
    mtx_unlock(&engine.mutex);

    return worker;
}

static void _engine_destroy_worker(engine_worker_t* worker) {
    mtx_lock(&engine.mutex);
    xcomm_list_remove(&worker->node);
    mtx_unlock(&engine.mutex);

    xcomm_event_loop_destroy(&worker->looper);

    _engine_worker_unref(worker);
}

static int _worker_thread(void* param) {
    (void)param;

    engine_worker_t* worker = _engine_create_worker();
    if (!worker) {
        return -1;
    }
    xcomm_event_loop_run(&worker->looper);
    
    _engine_destroy_worker(worker);

    xcomm_wg_done(&engine.waitgroup);
    return 0;
}

static void _engine_startup(int concurrency) {
    platform_socket_startup();

    int thrdcnt = (concurrency > 0) ? concurrency : 1;
    
    cnd_init(&engine.cond);
    mtx_init(&engine.mutex, mtx_plain);

    xcomm_wg_init(&engine.waitgroup);
    xcomm_list_init(&engine.workers);

    engine.roundrobin = _engine_roundrobin;
    
    for (int i = 0; i < thrdcnt; i++) {
        thrd_t tid;
        thrd_create(&tid, _worker_thread, NULL);
        thrd_detach(tid);
    }
    xcomm_wg_add(&engine.waitgroup, thrdcnt);
}

static void _engine_cleanup(void) {
    mtx_lock(&engine.mutex);
    xcomm_list_node_t* node = xcomm_list_head(&engine.workers);
    while (node != xcomm_list_sentinel(&engine.workers)) {
        engine_worker_t* worker = xcomm_list_data(node, engine_worker_t, node);
        node = xcomm_list_next(node);

        xcomm_event_loop_stop(&worker->looper);
    }
    mtx_unlock(&engine.mutex);

    xcomm_wg_wait(&engine.waitgroup);

    mtx_destroy(&engine.mutex);
    cnd_destroy(&engine.cond);
    platform_socket_cleanup();
}

void xcomm_startup(int concurrency, xcomm_dumper_config_t* conf) {
    if (!atomic_flag_test_and_set(&engine.initialized)) {
        _engine_startup(concurrency);
    }
}

void xcomm_cleanup(void) {
    if (atomic_flag_test_and_set(&engine.initialized)) {
        atomic_flag_clear(&engine.initialized);
        _engine_cleanup();
    }
}
