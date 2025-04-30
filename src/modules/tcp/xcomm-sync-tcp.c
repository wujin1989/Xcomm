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

#include "xcomm-sync-tcp.h"

void xcomm_sync_tcp_startup(void) {
    xcomm_socket_startup();
}

void xcomm_sync_tcp_cleanup(void) {
    xcomm_socket_cleanup();
}

xcomm_sock_t
xcomm_sync_tcp_dial(const char* restrict host, const char* restrict port) {
    bool connected;
    return xcomm_socket_dial(host, port, SOCK_STREAM, &connected, false);
}

xcomm_sock_t
xcomm_sync_tcp_listen(const char* restrict host, const char* restrict port) {
    return xcomm_socket_listen(host, port, SOCK_STREAM, 0, 0, false);
}

xcomm_sock_t xcomm_sync_tcp_accept(xcomm_sock_t sock) {
    return xcomm_socket_accept(sock, false);
}

int xcomm_sync_tcp_send(xcomm_sock_t sock, void* buf, int len) {
    return (int)xcomm_socket_sendall(sock, buf, len);
}

int xcomm_sync_tcp_recv(xcomm_sock_t sock, void* buf, int len) {
    return (int)xcomm_socket_recvall(sock, buf, len);
}

void xcomm_sync_tcp_close(xcomm_sock_t sock) {
    xcomm_socket_close(sock);
}

void xcomm_sync_tcp_sendtimeo(xcomm_sock_t sock, int timeout_ms) {
    xcomm_socket_sendtimeo(sock, timeout_ms);
}

void xcomm_sync_tcp_recvtimeo(xcomm_sock_t sock, int timeout_ms) {
    xcomm_socket_recvtimeo(sock, timeout_ms);
}