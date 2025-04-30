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

#include <stdatomic.h>

#include "xcomm-logger.h"
#include "internal-sync-tcp.h"
#include "xcomm/xcomm-sync-tcp-module.h"

static atomic_int refcnt;

static inline void _tcp_close(xcomm_socket_t* socketptr) {
    xcomm_logi("%s enter.\n", __FUNCTION__);

    xcomm_sock_t* sockptr = socketptr->opaque;
    xcomm_sock_t  sockobj = *sockptr;

    internal_sync_tcp_close(sockobj);
    free(sockptr);
    free(socketptr);

    atomic_fetch_sub(&refcnt, 1);
    if (!atomic_load(&refcnt)) {
        internal_sync_tcp_cleanup();
    }
    xcomm_logi("%s leave.\n", __FUNCTION__);
}

static inline xcomm_socket_t*
_tcp_dail(const char* restrict host, const char* restrict port) {
    xcomm_logi("%s enter.\n", __FUNCTION__);

    xcomm_socket_t* sockptr = malloc(sizeof(xcomm_socket_t));
    if (!sockptr) {
        xcomm_loge("no memory.\n");
        return NULL;
    }
    sockptr->opaque = malloc(sizeof(xcomm_sock_t));
    if (!sockptr->opaque) {
        xcomm_loge("no memory.\n");
        free(sockptr);
        return NULL;
    }
    if (!atomic_load(&refcnt)) {
        internal_sync_tcp_startup();
    }
    xcomm_sock_t sockobj = internal_sync_tcp_dial(host, port);
    memcpy(sockptr->opaque, &sockobj, sizeof(xcomm_sock_t));

    atomic_fetch_add(&refcnt, 1);

    xcomm_logi("%s leave.\n", __FUNCTION__);
    return sockptr;
}

static inline xcomm_socket_t*
_tcp_listen(const char* restrict host, const char* restrict port) {
    xcomm_logi("%s enter.\n", __FUNCTION__);

    xcomm_socket_t* sockptr = malloc(sizeof(xcomm_socket_t));
    if (!sockptr) {
        xcomm_loge("no memory.\n");
        return NULL;
    }
    sockptr->opaque = malloc(sizeof(xcomm_sock_t));
    if (!sockptr->opaque) {
        xcomm_loge("no memory.\n");
        free(sockptr);
        return NULL;
    }
    if (!atomic_load(&refcnt)) {
        internal_sync_tcp_startup();
    }
    xcomm_sock_t sockobj = internal_sync_tcp_listen(host, port);
    memcpy(sockptr->opaque, &sockobj, sizeof(xcomm_sock_t));

    atomic_fetch_add(&refcnt, 1);

    xcomm_logi("%s leave.\n", __FUNCTION__);
    return sockptr;
}

static inline xcomm_socket_t* _tcp_accept(xcomm_socket_t* socketptr) {
    xcomm_logi("%s enter.\n", __FUNCTION__);

    xcomm_sock_t* srvptr = socketptr->opaque;
    xcomm_sock_t  srvobj = *srvptr;

    xcomm_socket_t* cliptr = malloc(sizeof(xcomm_socket_t));
    if (!cliptr) {
        xcomm_loge("no memory.\n");
        return NULL;
    }
    cliptr->opaque = malloc(sizeof(xcomm_sock_t));
    if (!cliptr->opaque) {
        xcomm_loge("no memory.\n");
        free(cliptr);
        return NULL;
    }
    xcomm_sock_t cliobj = internal_sync_tcp_accept(srvobj);
    memcpy(cliptr->opaque, &cliobj, sizeof(xcomm_sock_t));

    atomic_fetch_add(&refcnt, 1);

    xcomm_logi("%s leave.\n", __FUNCTION__);
    return cliptr;
}

static inline int _tcp_send(xcomm_socket_t* socketptr, void* buf, int len) {
    xcomm_sock_t* sockptr = socketptr->opaque;
    xcomm_sock_t  sockobj = *sockptr;

    return internal_sync_tcp_send(sockobj, buf, len);
}

static inline int _tcp_recv(xcomm_socket_t* socketptr, void* buf, int len) {
    xcomm_sock_t* sockptr = socketptr->opaque;
    xcomm_sock_t  sockobj = *sockptr;

    return internal_sync_tcp_recv(sockobj, buf, len);
}

static inline void _tcp_sendtimeo(xcomm_socket_t* socketptr, int timeout_ms) {
    xcomm_logi("%s enter.\n", __FUNCTION__);

    xcomm_sock_t* sockptr = socketptr->opaque;
    xcomm_sock_t  sockobj = *sockptr;

    internal_sync_tcp_sendtimeo(sockobj, timeout_ms);

    xcomm_logi("%s leave.\n", __FUNCTION__);
}

static inline void _tcp_recvtimeo(xcomm_socket_t* socketptr, int timeout_ms) {
    xcomm_logi("%s enter.\n", __FUNCTION__);

    xcomm_sock_t* sockptr = socketptr->opaque;
    xcomm_sock_t  sockobj = *sockptr;

    internal_sync_tcp_recvtimeo(sockobj, timeout_ms);

    xcomm_logi("%s leave.\n", __FUNCTION__);
}

xcomm_sync_tcp_module_t xcomm_sync_tcp_module = {
    .name = "Xcomm sync tcp module",
    .xcomm_dial      = _tcp_dail,
    .xcomm_listen    = _tcp_listen,
    .xcomm_accept    = _tcp_accept,
    .xcomm_send      = _tcp_send,
    .xcomm_recv      = _tcp_recv,
    .xcomm_close     = _tcp_close,
    .xcomm_sendtimeo = _tcp_sendtimeo,
    .xcomm_recvtimeo = _tcp_recvtimeo,
};