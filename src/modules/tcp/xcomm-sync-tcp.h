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

#include "xcomm/xcomm-tcp-module.h"


extern xcomm_socket_t* xcomm_sync_tcp_dail(const char* restrict host, const char* restrict port);
extern xcomm_socket_t* xcomm_sync_tcp_listen(const char* restrict host, const char* restrict port);
extern xcomm_socket_t* xcomm_sync_tcp_accept(xcomm_socket_t* socketptr);
extern void xcomm_sync_tcp_close(xcomm_socket_t* socketptr);
extern int  xcomm_sync_tcp_send(xcomm_socket_t* socketptr, void* buf, int len);
extern int  xcomm_sync_tcp_recv(xcomm_socket_t* socketptr, void* buf, int len);
extern void xcomm_sync_tcp_sndtimeo_setup(xcomm_socket_t* socketptr, int timeout_ms);
extern void xcomm_sync_tcp_rcvtimeo_setup(xcomm_socket_t* socketptr, int timeout_ms);