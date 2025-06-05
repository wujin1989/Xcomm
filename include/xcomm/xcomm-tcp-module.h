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
typedef struct xcomm_tcp_connection_s   xcomm_tcp_connection_t;
typedef struct xcomm_tcp_listener_s     xcomm_tcp_listener_t;
typedef struct xcomm_tcp_packetizer_s   xcomm_tcp_packetizer_t;

typedef void (*xcomm_tcp_connect_cb_t)(
    xcomm_tcp_connection_t* conn,
    int                     error_code,
    const char*             error_message,
    void*                   userdata);

typedef void (*xcomm_tcp_listen_cb_t)(
    xcomm_tcp_listener_t* listener,
    int                   error_code,
    const char*           error_message,
    void*                 userdata);

typedef void (*xcomm_tcp_accept_cb_t)(
    xcomm_tcp_connection_t* conn, 
    int                     error_code,
    const char*             error_message,
    void*                   userdata);

typedef void (*xcomm_tcp_recv_cb_t)(
    xcomm_tcp_connection_t* conn, void* buf, size_t len, void* userdata);

typedef void (*xcomm_tcp_send_completed_cb_t)(
    xcomm_tcp_connection_t* conn, void* buf, size_t len, void* userdata);

typedef void (*xcomm_tcp_heartbeat_cb_t)(
    xcomm_tcp_connection_t* conn, void* userdata);

typedef void (*xcomm_tcp_connection_close_cb_t)(
    xcomm_tcp_connection_t* conn, void* userdata);

typedef void (*xcomm_tcp_listener_close_cb_t)(
    xcomm_tcp_listener_t* listener, void* userdata);

struct xcomm_tcp_connection_s {
    void* opaque;
};

struct xcomm_tcp_listener_s {
    void* opaque;
};

struct xcomm_tcp_packetizer_s {
    int a;
};

struct xcomm_sync_tcp_module_s {
    const char* restrict name;

    xcomm_tcp_connection_t* (*dial)(const char* restrict host, const char* restrict port);
    xcomm_tcp_listener_t* (*listen)(const char* restrict host, const char* restrict port);

    xcomm_tcp_connection_t* (*accept)(xcomm_tcp_listener_t* listener);
    void (*close_listener)(xcomm_tcp_listener_t* listener);

    int  (*send)(xcomm_tcp_connection_t* conn, void* buf, int len);
    int  (*recv)(xcomm_tcp_connection_t* conn, void* buf, int len);
    void (*close_connection)(xcomm_tcp_connection_t* conn);
    void (*set_sndtimeo)(xcomm_tcp_connection_t* conn, int timeout_ms);
    void (*set_rcvtimeo)(xcomm_tcp_connection_t* conn, int timeout_ms);
};

struct xcomm_async_tcp_module_s {
    const char* restrict name;

    void (*dial)(const char* restrict host, const char* restrict port, int timeout_ms, xcomm_tcp_connect_cb_t connect_cb, void* userdata);
    void (*listen)(const char* restrict host, const char* restrict port, xcomm_tcp_listen_cb_t listen_cb, void* userdata);
    
    void (*set_accept_cb)(xcomm_tcp_listener_t* listener, xcomm_tcp_accept_cb_t accept_cb, void* userdata);
    void (*set_listener_close_cb)(xcomm_tcp_listener_t* listener, xcomm_tcp_listener_close_cb_t listener_close_cb, void* userdata);
    void (*close_listener)(xcomm_tcp_listener_t* listener);

    void (*set_recv_cb)(xcomm_tcp_connection_t* conn, xcomm_tcp_recv_cb_t recv_cb, void* userdata);
    void (*set_send_completed_cb)(xcomm_tcp_connection_t* conn, xcomm_tcp_send_completed_cb_t send_completed_cb, void* userdata);
    void (*set_heartbeat_cb)(xcomm_tcp_connection_t* conn, xcomm_tcp_heartbeat_cb_t heartbeat_cb, void* userdata);
    void (*set_connection_close_cb)(xcomm_tcp_connection_t* conn, xcomm_tcp_connection_close_cb_t connection_close_cb, void* userdata);
    void (*close_connection)(xcomm_tcp_connection_t* conn);
    void (*send)(xcomm_tcp_connection_t* conn, void* buf, size_t len);
    void (*set_sendtimeo)(xcomm_tcp_connection_t* conn, int timeout_ms);
    void (*set_recvtimeo)(xcomm_tcp_connection_t* conn, int timeout_ms);
    void (*set_heartbeat_interval)(xcomm_tcp_connection_t* conn, int interval_ms);
    void (*set_packetizer)(xcomm_tcp_connection_t* conn,xcomm_tcp_packetizer_t* packetizer);
};

extern xcomm_sync_tcp_module_t  xcomm_sync_tcp;
extern xcomm_async_tcp_module_t xcomm_async_tcp;