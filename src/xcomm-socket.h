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

_Pragma("once")

#include "platform/platform-types.h"

#define XCOMM_SO_ERROR_EAGAIN         PLATFORM_SO_ERROR_EAGAIN
#define XCOMM_SO_ERROR_EWOULDBLOCK    PLATFORM_SO_ERROR_EWOULDBLOCK
#define XCOMM_SO_ERROR_ECONNRESET     PLATFORM_SO_ERROR_ECONNRESET
#define XCOMM_SO_ERROR_ETIMEDOUT      PLATFORM_SO_ERROR_ETIMEDOUT
#define XCOMM_SO_ERROR_INVALID_SOCKET PLATFORM_SO_ERROR_INVALID_SOCKET
#define XCOMM_SO_ERROR_SOCKET_ERROR   PLATFORM_SO_ERROR_SOCKET_ERROR

typedef platform_sock_t   xcomm_sock_t;
typedef platform_pollfd_t xcomm_pollfd_t;

extern void    xcomm_socket_recvtimeo(xcomm_sock_t sock, int timeout_ms);
extern void    xcomm_socket_sendtimeo(xcomm_sock_t sock, int timeout_ms);
extern void    xcomm_socket_setrecvbuf(xcomm_sock_t sock, int val);
extern void    xcomm_socket_setsendbuf(xcomm_sock_t sock, int val);
extern void    xcomm_socket_nodelay(xcomm_sock_t sock, bool on);
extern void    xcomm_socket_v6only(xcomm_sock_t sock, bool on);
extern void    xcomm_socket_rss(xcomm_sock_t sock, uint16_t idx, int cores);
extern void    xcomm_socket_keepalive(xcomm_sock_t sock);
extern void    xcomm_socket_maxseg(xcomm_sock_t sock);
extern void    xcomm_socket_nonblock(xcomm_sock_t sock);
extern void    xcomm_socket_reuse_addr(xcomm_sock_t sock);
extern void    xcomm_socket_reuse_port(xcomm_sock_t sock);
extern int     xcomm_socket_extract_family(xcomm_sock_t sock);
extern void    xcomm_socket_startup(void);
extern void    xcomm_socket_cleanup(void);
extern void    xcomm_socket_close(xcomm_sock_t sock);
extern int     xcomm_socket_getaddrfamily(xcomm_sock_t sock);
extern int     xcomm_socket_getsocktype(xcomm_sock_t sock);
extern ssize_t xcomm_socket_recv(xcomm_sock_t sock, void* buf, int size);
extern ssize_t xcomm_socket_send(xcomm_sock_t sock, void* buf, int size);
extern ssize_t xcomm_socket_recvall(xcomm_sock_t sock, void* buf, int size);
extern ssize_t xcomm_socket_sendall(xcomm_sock_t sock, void* buf, int size);
extern ssize_t xcomm_socket_recvfrom(xcomm_sock_t sock, void* buf, int size, struct sockaddr_storage* ss, socklen_t* sslen);
extern ssize_t xcomm_socket_sendto(xcomm_sock_t sock, void* buf, int size, struct sockaddr_storage* ss, socklen_t sslen);
extern int     xcomm_socket_socketpair(int domain, int type, int protocol, xcomm_sock_t socks[2]);
extern char*   xcomm_socket_error2string(int error);
extern int     xcomm_socket_lasterror(void);
extern void    xcomm_socket_pollfd_destroy(xcomm_pollfd_t pfd);
extern xcomm_pollfd_t xcomm_socket_pollfd_create(void);
extern xcomm_sock_t   xcomm_socket_accept(xcomm_sock_t sock, bool nonblocking);
extern xcomm_sock_t   xcomm_socket_listen(const char* restrict host, const char* restrict port, int protocol, int idx, int cores, bool nonblocking);
extern xcomm_sock_t   xcomm_socket_dial(const char* restrict host, const char* restrict port, int protocol, bool* connected, bool nonblocking);