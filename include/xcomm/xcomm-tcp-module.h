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

_Pragma("once")

typedef struct xcomm_sync_tcp_module_s  xcomm_sync_tcp_module_t;
typedef struct xcomm_async_tcp_module_s xcomm_async_tcp_module_t;
typedef struct xcomm_socket_s           xcomm_socket_t;

struct xcomm_socket_s {
    void* opaque;
};

struct xcomm_sync_tcp_module_s {
    const char* restrict name;

    xcomm_socket_t* (*dial)(const char* restrict host, const char* restrict port);
    xcomm_socket_t* (*listen)(const char* restrict host, const char* restrict port);
    xcomm_socket_t* (*accept)(xcomm_socket_t* socket);
    int  (*send)(xcomm_socket_t* sock, void* buf, int len);
    int  (*recv)(xcomm_socket_t* sock, void* buf, int len);
    void (*close)(xcomm_socket_t* sock);
    void (*set_sndtimeo)(xcomm_socket_t* sock, int timeout_ms);
    void (*set_rcvtimeo)(xcomm_socket_t* sock, int timeout_ms);
};

struct xcomm_async_tcp_module_s {
    const char* restrict name;
};

extern xcomm_sync_tcp_module_t  xcomm_sync_tcp;
extern xcomm_async_tcp_module_t xcomm_async_tcp;