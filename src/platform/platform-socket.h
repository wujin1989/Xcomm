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

#include "platform-types.h"

extern void    platform_socket_startup(void);
extern void    platform_socket_cleanup(void);
extern void    platform_socket_close(platform_sock_t sock);
extern ssize_t platform_socket_recv(platform_sock_t sock, void* buf, int size);
extern ssize_t platform_socket_send(platform_sock_t sock, void* buf, int size);
extern ssize_t platform_socket_recvall(platform_sock_t sock, void* buf, int size);
extern ssize_t platform_socket_sendall(platform_sock_t sock, void* buf, int size);
extern ssize_t platform_socket_recvfrom(platform_sock_t sock, void* buf, int size, struct sockaddr_storage* ss, socklen_t* sslen);
extern ssize_t platform_socket_sendto(platform_sock_t sock, void* buf, int size, struct sockaddr_storage* ss, socklen_t sslen);
extern int     platform_socket_socketpair(int domain, int type, int protocol, platform_sock_t socks[2]);
extern char*   platform_socket_tostring(int error);
extern platform_sock_t platform_socket_accept(platform_sock_t sock, bool nonblocking);
extern platform_sock_t platform_socket_listen(const char* restrict host, const char* restrict port, int protocol, int idx, int cores, bool nonblocking);
extern platform_sock_t platform_socket_dial(const char* restrict host, const char* restrict port, int protocol, bool* connected, bool  nonblocking);

extern void platform_socket_set_rcvtimeout(platform_sock_t sock, int timeout_ms);
extern void platform_socket_set_sndtimeout(platform_sock_t sock, int timeout_ms);
extern void platform_socket_set_rcvbuf(platform_sock_t sock, int val);
extern void platform_socket_set_sndbuf(platform_sock_t sock, int val);
extern void platform_socket_set_rss(platform_sock_t sock, uint16_t idx, int cores);
extern int  platform_socket_get_addressfamily(platform_sock_t sock);
extern int  platform_socket_get_socktype(platform_sock_t sock);
extern int  platform_socket_get_lasterror(void);

extern void platform_socket_enable_nodelay(platform_sock_t sock, bool on);
extern void platform_socket_enable_v6only(platform_sock_t sock, bool on);
extern void platform_socket_enable_keepalive(platform_sock_t sock, bool on);
extern void platform_socket_enable_maxseg(platform_sock_t sock, bool on);
extern void platform_socket_enable_nonblocking(platform_sock_t sock, bool on);
extern void platform_socket_enable_reuseaddr(platform_sock_t sock, bool on);
extern void platform_socket_enable_reuseport(platform_sock_t sock, bool on);








