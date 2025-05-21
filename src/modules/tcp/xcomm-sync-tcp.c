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

#include "xcomm-logger.h"
#include "xcomm-sync-tcp.h"
#include "platform/platform-socket.h"

static atomic_int refcnt;

void xcomm_sync_tcp_close(xcomm_socket_t* socketptr) {
    xcomm_logi("%s enter.\n", __FUNCTION__);

    platform_sock_t* sockptr = socketptr->opaque;
    platform_sock_t  sockobj = *sockptr;

    platform_socket_close(sockobj);
    free(sockptr);
    free(socketptr);

    atomic_fetch_sub(&refcnt, 1);
    if (!atomic_load(&refcnt)) {
        platform_socket_cleanup();
    }
    xcomm_logi("%s leave.\n", __FUNCTION__);
}

xcomm_socket_t* xcomm_sync_tcp_dial(const char* restrict host, const char* restrict port) {
    xcomm_logi("%s enter.\n", __FUNCTION__);

    xcomm_socket_t* sockptr = malloc(sizeof(xcomm_socket_t));
    if (!sockptr) {
        xcomm_loge("no memory.\n");
        return NULL;
    }
    sockptr->opaque = malloc(sizeof(platform_sock_t));
    if (!sockptr->opaque) {
        xcomm_loge("no memory.\n");
        free(sockptr);
        return NULL;
    }
    if (!atomic_load(&refcnt)) {
        platform_socket_startup();
    }
    bool noused;
    platform_sock_t sockobj =
        platform_socket_dial(host, port, SOCK_STREAM, &noused, false);
    if (sockobj != PLATFORM_SO_ERROR_INVALID_SOCKET) {
        memcpy(sockptr->opaque, &sockobj, sizeof(platform_sock_t));
    } else {
        xcomm_loge("tcp dial error.\n");
        free(sockptr->opaque);
        free(sockptr);
        return NULL;
    }
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
    sockptr->opaque = malloc(sizeof(platform_sock_t));
    if (!sockptr->opaque) {
        xcomm_loge("no memory.\n");
        free(sockptr);
        return NULL;
    }
    if (!atomic_load(&refcnt)) {
        platform_socket_startup();
    }
    platform_sock_t sockobj =
        platform_socket_listen(host, port, SOCK_STREAM, 0, 0, false);
    if (sockobj != PLATFORM_SO_ERROR_INVALID_SOCKET) {
        memcpy(sockptr->opaque, &sockobj, sizeof(platform_sock_t));
    } else {
        xcomm_loge("tcp listen error.\n");
        free(sockptr->opaque);
        free(sockptr);
        return NULL;
    }
    atomic_fetch_add(&refcnt, 1);

    xcomm_logi("%s leave.\n", __FUNCTION__);
    return sockptr;
}

xcomm_socket_t* xcomm_sync_tcp_accept(xcomm_socket_t* socketptr) {
    xcomm_logi("%s enter.\n", __FUNCTION__);

    platform_sock_t* srvptr = socketptr->opaque;
    platform_sock_t  srvobj = *srvptr;

    xcomm_socket_t* cliptr = malloc(sizeof(xcomm_socket_t));
    if (!cliptr) {
        xcomm_loge("no memory.\n");
        return NULL;
    }
    cliptr->opaque = malloc(sizeof(platform_sock_t));
    if (!cliptr->opaque) {
        xcomm_loge("no memory.\n");
        free(cliptr);
        return NULL;
    }
    platform_sock_t cliobj = platform_socket_accept(srvobj, false);
    if (cliobj != PLATFORM_SO_ERROR_INVALID_SOCKET) {
        memcpy(cliptr->opaque, &cliobj, sizeof(platform_sock_t));
    } else {
        xcomm_loge("tcp accept error.\n");
        free(cliptr->opaque);
        free(cliptr);
        return NULL;
    }
    atomic_fetch_add(&refcnt, 1);

    xcomm_logi("%s leave.\n", __FUNCTION__);
    return cliptr;
}

int xcomm_sync_tcp_send(xcomm_socket_t* socketptr, void* buf, int len) {
    platform_sock_t* sockptr = socketptr->opaque;
    platform_sock_t  sockobj = *sockptr;

    int ret = (int)platform_socket_sendall(sockobj, buf, len);
    if (ret == PLATFORM_SO_ERROR_SOCKET_ERROR) {
        return -1;
    }
    return ret;
}

int xcomm_sync_tcp_recv(xcomm_socket_t* socketptr, void* buf, int len) {
    platform_sock_t* sockptr = socketptr->opaque;
    platform_sock_t  sockobj = *sockptr;

    int ret = (int)platform_socket_recvall(sockobj, buf, len);
    if (ret == PLATFORM_SO_ERROR_SOCKET_ERROR) {
        return -1;
    }
    return ret;
}

void xcomm_sync_tcp_set_sndtimeout(xcomm_socket_t* socketptr, int timeout_ms) {
    xcomm_logi("%s enter.\n", __FUNCTION__);

    platform_sock_t* sockptr = socketptr->opaque;
    platform_sock_t  sockobj = *sockptr;

    platform_socket_set_sndtimeout(sockobj, timeout_ms);

    xcomm_logi("%s leave.\n", __FUNCTION__);
}

void xcomm_sync_tcp_set_rcvtimeout(xcomm_socket_t* socketptr, int timeout_ms) {
    xcomm_logi("%s enter.\n", __FUNCTION__);

    platform_sock_t* sockptr = socketptr->opaque;
    platform_sock_t  sockobj = *sockptr;

    platform_socket_set_rcvtimeout(sockobj, timeout_ms);

    xcomm_logi("%s leave.\n", __FUNCTION__);
}