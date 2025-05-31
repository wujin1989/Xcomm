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

#include "xcomm-logger.h"
#include "xcomm-sync-tcp.h"
#include "platform/platform-socket.h"

void xcomm_sync_tcp_destroy_listener(xcomm_tcp_listener_t* listener) {
    xcomm_logi("%s enter.\n", __FUNCTION__);

    platform_sock_t* sock = listener->opaque;
    platform_socket_close(*sock);
    free(listener->opaque);
    free(listener);

    xcomm_logi("%s leave.\n", __FUNCTION__);
}

void xcomm_sync_tcp_destroy_connection(xcomm_tcp_connection_t* conn) {
    xcomm_logi("%s enter.\n", __FUNCTION__);

    platform_sock_t* sock = conn->opaque;
    platform_socket_close(*sock);

    free(conn->opaque);
    free(conn);

    xcomm_logi("%s leave.\n", __FUNCTION__);
}

xcomm_tcp_connection_t*
xcomm_sync_tcp_dial(const char* restrict host, const char* restrict port) {
    xcomm_logi("%s enter.\n", __FUNCTION__);

    xcomm_tcp_connection_t* conn = malloc(sizeof(xcomm_tcp_connection_t));
    if (!conn) {
        xcomm_loge("no memory.\n");
        return NULL;
    }
    conn->opaque = malloc(sizeof(platform_sock_t));
    if (!conn->opaque) {
        xcomm_loge("no memory.\n");
        free(conn);
        return NULL;
    }
    bool noused;
    platform_sock_t sock =
        platform_socket_dial(host, port, SOCK_STREAM, &noused, false);
    if (sock != PLATFORM_SO_ERROR_INVALID_SOCKET) {
        memcpy(conn->opaque, &sock, sizeof(platform_sock_t));
    } else {
        xcomm_loge("tcp dial error.\n");
        free(conn->opaque);
        free(conn);
        return NULL;
    }
    xcomm_logi("%s leave.\n", __FUNCTION__);
    return conn;
}

xcomm_tcp_listener_t*
xcomm_sync_tcp_listen(const char* restrict host, const char* restrict port) {
    xcomm_logi("%s enter.\n", __FUNCTION__);

    xcomm_tcp_listener_t* listener = malloc(sizeof(xcomm_tcp_listener_t));
    if (!listener) {
        xcomm_loge("no memory.\n");
        return NULL;
    }
    listener->opaque = malloc(sizeof(platform_sock_t));
    if (!listener->opaque) {
        xcomm_loge("no memory.\n");
        free(listener);
        return NULL;
    }
    platform_sock_t sock =
        platform_socket_listen(host, port, SOCK_STREAM, 0, 0, false);
    if (sock != PLATFORM_SO_ERROR_INVALID_SOCKET) {
        memcpy(listener->opaque, &sock, sizeof(platform_sock_t));
    } else {
        xcomm_loge("tcp listen error.\n");
        free(listener->opaque);
        free(listener);
        return NULL;
    }
    xcomm_logi("%s leave.\n", __FUNCTION__);
    return listener;
}

xcomm_tcp_connection_t* xcomm_sync_tcp_accept(xcomm_tcp_listener_t* listener) {
    xcomm_logi("%s enter.\n", __FUNCTION__);

    platform_sock_t* srv_sock = listener->opaque;

    xcomm_tcp_connection_t* conn = malloc(sizeof(xcomm_tcp_connection_t));
    if (!conn) {
        xcomm_loge("no memory.\n");
        return NULL;
    }
    conn->opaque = malloc(sizeof(platform_sock_t));
    if (!conn->opaque) {
        xcomm_loge("no memory.\n");
        free(conn);
        return NULL;
    }
    platform_sock_t cli_sock = platform_socket_accept(*srv_sock, false);
    if (cli_sock != PLATFORM_SO_ERROR_INVALID_SOCKET) {
        memcpy(conn->opaque, &cli_sock, sizeof(platform_sock_t));
    } else {
        xcomm_loge("tcp accept error.\n");
        free(conn->opaque);
        free(conn);
        return NULL;
    }
    xcomm_logi("%s leave.\n", __FUNCTION__);
    return conn;
}

int xcomm_sync_tcp_send(xcomm_tcp_connection_t* conn, void* buf, int len) {
    platform_sock_t* sock = conn->opaque;

    int ret = (int)platform_socket_sendall(*sock, buf, len);
    if (ret == PLATFORM_SO_ERROR_SOCKET_ERROR) {
        return -1;
    }
    return ret;
}

int xcomm_sync_tcp_recv(xcomm_tcp_connection_t* conn, void* buf, int len) {
    platform_sock_t* sock = conn->opaque;

    int ret = (int)platform_socket_recvall(*sock, buf, len);
    if (ret == PLATFORM_SO_ERROR_SOCKET_ERROR) {
        return -1;
    }
    return ret;
}

void xcomm_sync_tcp_set_sndtimeout(
    xcomm_tcp_connection_t* conn, int timeout_ms) {
    xcomm_logi("%s enter.\n", __FUNCTION__);

    platform_sock_t* sock = conn->opaque;
    platform_socket_set_sndtimeout(*sock, timeout_ms);

    xcomm_logi("%s leave.\n", __FUNCTION__);
}

void xcomm_sync_tcp_set_rcvtimeout(
    xcomm_tcp_connection_t* conn, int timeout_ms) {
    xcomm_logi("%s enter.\n", __FUNCTION__);

    platform_sock_t* sock = conn->opaque;
    platform_socket_set_rcvtimeout(*sock, timeout_ms);

    xcomm_logi("%s leave.\n", __FUNCTION__);
}