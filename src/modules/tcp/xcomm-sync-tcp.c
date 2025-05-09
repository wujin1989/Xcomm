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

#include "xcomm-socket.h"
#include "xcomm-logger.h"
#include "xcomm-sync-tcp.h"

static atomic_int refcnt;

void xcomm_sync_tcp_close(xcomm_socket_t* socketptr) {
    xcomm_logi("%s enter.\n", __FUNCTION__);

    xcomm_sock_t* sockptr = socketptr->opaque;
    xcomm_sock_t  sockobj = *sockptr;

    xcomm_socket_close(sockobj);
    free(sockptr);
    free(socketptr);

    atomic_fetch_sub(&refcnt, 1);
    if (!atomic_load(&refcnt)) {
        xcomm_socket_cleanup();
    }
    xcomm_logi("%s leave.\n", __FUNCTION__);
}

xcomm_socket_t* xcomm_sync_tcp_dail(const char* restrict host, const char* restrict port) {
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
        xcomm_socket_startup();
    }
    bool nouse;
    xcomm_sock_t sockobj =
        xcomm_socket_dial(host, port, SOCK_STREAM, &nouse, false);
    memcpy(sockptr->opaque, &sockobj, sizeof(xcomm_sock_t));

    atomic_fetch_add(&refcnt, 1);

    xcomm_logi("%s leave.\n", __FUNCTION__);
    return sockptr;
}

xcomm_socket_t*
xcomm_sync_tcp_listen(const char* restrict host, const char* restrict port) {
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
        xcomm_sync_tcp_startup();
    }
    xcomm_sock_t sockobj =
        xcomm_socket_listen(host, port, SOCK_STREAM, 0, 0, false);
    memcpy(sockptr->opaque, &sockobj, sizeof(xcomm_sock_t));

    atomic_fetch_add(&refcnt, 1);

    xcomm_logi("%s leave.\n", __FUNCTION__);
    return sockptr;
}

xcomm_socket_t* xcomm_sync_tcp_accept(xcomm_socket_t* socketptr) {
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
    xcomm_sock_t cliobj = xcomm_socket_accept(srvobj, false);
    memcpy(cliptr->opaque, &cliobj, sizeof(xcomm_sock_t));

    atomic_fetch_add(&refcnt, 1);

    xcomm_logi("%s leave.\n", __FUNCTION__);
    return cliptr;
}

int xcomm_sync_tcp_send(xcomm_socket_t* socketptr, void* buf, int len) {
    xcomm_sock_t* sockptr = socketptr->opaque;
    xcomm_sock_t  sockobj = *sockptr;

    return (int)xcomm_socket_sendall(sockobj, buf, len);
}

int xcomm_sync_tcp_recv(xcomm_socket_t* socketptr, void* buf, int len) {
    xcomm_sock_t* sockptr = socketptr->opaque;
    xcomm_sock_t  sockobj = *sockptr;

    return (int)xcomm_socket_recvall(sockobj, buf, len);
}

void xcomm_sync_tcp_sndtimeo_setup(xcomm_socket_t* socketptr, int timeout_ms) {
    xcomm_logi("%s enter.\n", __FUNCTION__);

    xcomm_sock_t* sockptr = socketptr->opaque;
    xcomm_sock_t  sockobj = *sockptr;

    xcomm_socket_sendtimeo(sockobj, timeout_ms);

    xcomm_logi("%s leave.\n", __FUNCTION__);
}

void xcomm_sync_tcp_rcvtimeo_setup(xcomm_socket_t* socketptr, int timeout_ms) {
    xcomm_logi("%s enter.\n", __FUNCTION__);

    xcomm_sock_t* sockptr = socketptr->opaque;
    xcomm_sock_t  sockobj = *sockptr;

    xcomm_socket_recvtimeo(sockobj, timeout_ms);

    xcomm_logi("%s leave.\n", __FUNCTION__);
}