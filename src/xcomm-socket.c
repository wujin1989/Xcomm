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

#include "xcomm-socket.h"
#include "platform/platform-socket.h"

void xcomm_socket_recvtimeo(xcomm_sock_t sock, int timeout_ms) {
    platform_socket_recvtimeo(sock, timeout_ms);
}

void xcomm_socket_sendtimeo(xcomm_sock_t sock, int timeout_ms) {
    platform_socket_sendtimeo(sock, timeout_ms);
}

void xcomm_socket_setrecvbuf(xcomm_sock_t sock, int val) {
    platform_socket_setrecvbuf(sock, val);
}

void xcomm_socket_setsendbuf(xcomm_sock_t sock, int val) {
    platform_socket_setsendbuf(sock, val);
}

void xcomm_socket_nodelay(xcomm_sock_t sock, bool on) {
    platform_socket_nodelay(sock, on);
}

void xcomm_socket_v6only(xcomm_sock_t sock, bool on) {
    platform_socket_v6only(sock, on);
}

void xcomm_socket_rss(xcomm_sock_t sock, uint16_t idx, int cores) {
    platform_socket_rss(sock, idx, cores);
}

void xcomm_socket_keepalive(xcomm_sock_t sock) {
    platform_socket_keepalive(sock);
}

void xcomm_socket_maxseg(xcomm_sock_t sock) {
    platform_socket_maxseg(sock);
}

void xcomm_socket_nonblock(xcomm_sock_t sock) {
    platform_socket_nonblock(sock);
}

void xcomm_socket_reuse_addr(xcomm_sock_t sock) {
    platform_socket_reuse_addr(sock);
}

void xcomm_socket_reuse_port(xcomm_sock_t sock) {
    platform_socket_reuse_port(sock);
}

int xcomm_socket_extract_family(xcomm_sock_t sock) {
    return platform_socket_extract_family(sock);
}

void xcomm_socket_startup(void) {
    platform_socket_startup();
}

void xcomm_socket_cleanup(void) {
    platform_socket_cleanup();
}

void xcomm_socket_close(xcomm_sock_t sock) {
    platform_socket_close(sock);
}

int xcomm_socket_getaddrfamily(xcomm_sock_t sock) {
    return platform_socket_getaddrfamily(sock);
}

int xcomm_socket_getsocktype(xcomm_sock_t sock) {
    return platform_socket_getsocktype(sock);
}

ssize_t xcomm_socket_recv(xcomm_sock_t sock, void* buf, int size) {
    return platform_socket_recv(sock, buf, size);
}

ssize_t xcomm_socket_send(xcomm_sock_t sock, void* buf, int size) {
    return platform_socket_send(sock, buf, size);
}

ssize_t xcomm_socket_recvall(xcomm_sock_t sock, void* buf, int size) {
    return platform_socket_recvall(sock, buf, size);
}

ssize_t xcomm_socket_sendall(xcomm_sock_t sock, void* buf, int size) {
    return platform_socket_sendall(sock, buf, size);
}

ssize_t xcomm_socket_recvfrom(
    xcomm_sock_t             sock,
    void*                    buf,
    int                      size,
    struct sockaddr_storage* ss,
    socklen_t*               sslen) {
    return platform_socket_recvfrom(sock, buf, size, ss, sslen);
}

ssize_t xcomm_socket_sendto(
    xcomm_sock_t             sock,
    void*                    buf,
    int                      size,
    struct sockaddr_storage* ss,
    socklen_t                sslen) {
    return platform_socket_sendto(sock, buf, size, ss, sslen);
}

int xcomm_socket_socketpair(
    int domain, int type, int protocol, xcomm_sock_t socks[2]) {
    return platform_socket_socketpair(domain, type, protocol, socks);
}

char* xcomm_socket_error2string(int error) {
    return platform_socket_error2string(error);
}

int xcomm_socket_lasterror(void) {
    return platform_socket_lasterror();
}

void xcomm_socket_pollfd_destroy(xcomm_pollfd_t pfd) {
    platform_socket_pollfd_destroy(pfd);
}

xcomm_pollfd_t xcomm_socket_pollfd_create(void) {
    return platform_socket_pollfd_create();
}

xcomm_sock_t xcomm_socket_accept(xcomm_sock_t sock, bool nonblocking) {
    return platform_socket_accept(sock, nonblocking);
}

xcomm_sock_t xcomm_socket_listen(
      const char* restrict host,
      const char* restrict port,
      int  protocol,
      int  idx,
      int  cores,
    bool nonblocking) {
    return platform_socket_listen(
        host, port, protocol, idx, cores, nonblocking);
}

xcomm_sock_t xcomm_socket_dial(
    const char* restrict host,
    const char* restrict port,
    int   protocol,
    bool* connected,
    bool  nonblocking) {
    return platform_socket_dial(host, port, protocol, connected, nonblocking);
}