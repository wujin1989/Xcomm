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
    void (*dial)(const char* restrict host, const char* restrict port, xcomm_handler_t* handler);
    void (*listen)(const char* restrict host, const char* restrict port, xcomm_handler_t* handler);
    bool (*send)(xcomm_channel_t* channel, void* buf, size_t len);
    void (*close)(xcomm_channel_t* channel);
    void (*post_task)(void (*routine)(void*), void* param);
    void (*add_timer)(void (*routine)(void*), void* param, size_t expire, bool repeat);
};

extern xcomm_sync_tcp_module_t  xcomm_sync_tcp;
extern xcomm_async_tcp_module_t xcomm_async_tcp;



extern void cdk_net_ntop(struct sockaddr_storage* ss, cdk_address_t* ai);
extern void cdk_net_pton(cdk_address_t* ai, struct sockaddr_storage* ss);
extern void cdk_net_address_make(cdk_sock_t sock, struct sockaddr_storage* ss, char* host, char* port);
extern void cdk_net_address_retrieve(cdk_sock_t sock, cdk_address_t* ai, bool peer);
extern void cdk_net_concurrency_configure(int ncpus); 
extern void cdk_net_listen(const char* protocol, const char* host, const char* port, cdk_handler_t* handler);
extern void cdk_net_dial(const char* protocol, const char* host, const char* port, cdk_handler_t* handler);
extern bool cdk_net_send(cdk_channel_t* channel, void* data, size_t size);
extern void cdk_net_post_event(cdk_poller_t* poller, void (*task)(void*), void* arg, bool totail);
extern void cdk_net_timer_create(void (*routine)(void*), void* param, size_t expire, bool repeat);
extern void cdk_net_close(cdk_channel_t* channel);
extern void cdk_net_exit(void);