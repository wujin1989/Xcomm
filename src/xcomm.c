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

#include "xcomm.h"
#include "xcomm-wg.h"
#include "xcomm-event-loop.h"
#include "deprecated/c11-threads.h"

#include "platform/platform-socket.h"

typedef struct xcomm_engine_s xcomm_engine_t;

struct xcomm_engine_s {
    thrd_t*              thrdids;
    atomic_int           thrdcnt;
    atomic_flag          initialized;
    xcomm_event_loop_t** loopers;
    xcomm_wg_t*          loopers_wg;
    mtx_t                loopers_mtx;
    cnd_t                loopers_cnd;
    xcomm_event_loop_t* (*roundrobin)(void);
};

xcomm_engine_t engine = {.initialized = ATOMIC_FLAG_INIT};

static xcomm_event_loop_t* _roundrobin(void) {
    static atomic_int next = 0;
    return engine
        .loopers[atomic_fetch_add(&next, 1) % atomic_load(&engine.thrdcnt)];
}

static int _event_loop_thread(void* param) {
    xcomm_event_loop_t* loop = (xcomm_event_loop_t*)param;
    
    loop->tid = thrd_current();

    xcomm_event_loop_run(loop);
    xcomm_event_loop_destroy(loop);

    xcomm_wg_done(engine.loopers_wg);
    return 0;
}

static void _engine_startup(int concurrency) {
    platform_socket_startup();

    atomic_store(&engine.thrdcnt, concurrency > 0 ? concurrency : 1);

    mtx_init(&engine.loopers_mtx, mtx_plain);
    cnd_init(&engine.loopers_cnd);

    xcomm_wg_init(&engine.loopers_wg);
    xcomm_wg_add(&engine.loopers_wg, engine.thrdcnt);

    int thrdcnt = atomic_load(&engine.thrdcnt);
    engine.thrdids = malloc(thrdcnt * sizeof(thrd_t));
    engine.loopers = malloc(thrdcnt * sizeof(xcomm_event_loop_t*));

    if (!engine.thrdids || !engine.loopers) {
        return;
    }
    for (int i = 0; i < engine.thrdcnt; i++) {
        xcomm_event_loop_init(engine.loopers[i]);

        thrd_create(&engine.thrdids[i], _event_loop_thread, engine.loopers[i]);
        thrd_detach(engine.thrdids[i]);
    }
    engine.roundrobin = _roundrobin;
}

static void _engine_cleanup(void) {
    platform_socket_cleanup();
}

void xcomm_startup(int concurrency) {
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
