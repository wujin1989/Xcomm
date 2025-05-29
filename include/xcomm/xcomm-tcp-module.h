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

#include <stddef.h>
#include <stdbool.h>

typedef struct xcomm_sync_tcp_module_s  xcomm_sync_tcp_module_t;
typedef struct xcomm_async_tcp_module_s xcomm_async_tcp_module_t;
typedef struct xcomm_tcp_socket_s       xcomm_tcp_socket_t;
typedef struct xcomm_tcp_channel_s      xcomm_tcp_channel_t;
typedef struct xcomm_tcp_packetizer_s   xcomm_tcp_packetizer_t;

struct xcomm_tcp_socket_s {
    void* opaque;
};

struct xcomm_tcp_channel_s {
    void* opaque;
};

struct xcomm_tcp_packetizer_s {

};

struct xcomm_sync_tcp_module_s {
    const char* restrict name;

    xcomm_tcp_socket_t* (*dial)(const char* restrict host, const char* restrict port);
    xcomm_tcp_socket_t* (*listen)(const char* restrict host, const char* restrict port);
    xcomm_tcp_socket_t* (*accept)(xcomm_tcp_socket_t* socket);
    int  (*send)(xcomm_tcp_socket_t* sock, void* buf, int len);
    int  (*recv)(xcomm_tcp_socket_t* sock, void* buf, int len);
    void (*close)(xcomm_tcp_socket_t* sock);
    void (*set_sndtimeo)(xcomm_tcp_socket_t* sock, int timeout_ms);
    void (*set_rcvtimeo)(xcomm_tcp_socket_t* sock, int timeout_ms);
};

struct xcomm_async_tcp_module_s {
    const char* restrict name;

    void (*dial)(const char* restrict host, const char* restrict port, void (*on_connect)(xcomm_tcp_channel_t* channel, void* userdata), void* userdata);
    void (*listen)(const char* restrict host, const char* restrict port, void (*on_accept)(xcomm_tcp_channel_t* channel, void* userdata), void* userdata);

    void (*set_recv_cb)(xcomm_tcp_channel_t* channel, void (*on_recv)(xcomm_tcp_channel_t* channel, void* buf, size_t len, void* userdata), void* userdata);
    void (*set_send_cb)(xcomm_tcp_channel_t* channel, void (*on_send_complete)(xcomm_tcp_channel_t* channel, void* buf, size_t len, void* userdata), void* userdata);
    void (*set_heartbeat_cb)(xcomm_tcp_channel_t* channel, void (*on_heartbeat)(xcomm_tcp_channel_t* channel, void* userdata), void* userdata);
    void (*set_close_cb)(xcomm_tcp_channel_t* channel, void (*on_close)(xcomm_tcp_channel_t* channel, void* userdata), void* userdata);

    void (*send)(xcomm_tcp_channel_t* channel, void* buf, size_t len);
    void (*close)(xcomm_tcp_channel_t* channel);

    void (*set_conntimeo)(xcomm_tcp_channel_t* channel, int timeout_ms);
    void (*set_sendtimeo)(xcomm_tcp_channel_t* channel, int timeout_ms);
    void (*set_recvtimeo)(xcomm_tcp_channel_t* channel, int timeout_ms);
    void (*set_heartbeat_interval)(xcomm_tcp_channel_t* channel, int interval_ms);
    
    void (*set_packetizer)(xcomm_tcp_packetizer_t* packetizer);
};

extern xcomm_sync_tcp_module_t  xcomm_sync_tcp;
extern xcomm_async_tcp_module_t xcomm_async_tcp;