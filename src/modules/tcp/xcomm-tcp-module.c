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

#include "xcomm-sync-tcp.h"
#include "xcomm-async-tcp.h"
#include "xcomm/xcomm-tcp-module.h"

xcomm_sync_tcp_module_t xcomm_sync_tcp = {
    .name               = "Xcomm Sync TCP Module",

    .dial               = xcomm_sync_tcp_dial,
    .listen             = xcomm_sync_tcp_listen,

    .accept             = xcomm_sync_tcp_accept,
    .close_listener     = xcomm_sync_tcp_close_listener,

    .send               = xcomm_sync_tcp_send,
    .recv               = xcomm_sync_tcp_recv,
    .close_connection   = xcomm_sync_tcp_close_connection,
    .set_sndtimeo       = xcomm_sync_tcp_set_sndtimeout,
    .set_rcvtimeo       = xcomm_sync_tcp_set_rcvtimeout,
};

xcomm_async_tcp_module_t xcomm_async_tcp = {
    .name                      = "Xcomm Async TCP Module",

    .dial                      = xcomm_async_tcp_dial,
    .listen                    = xcomm_async_tcp_listen,

    .set_accept_cb             = xcomm_async_tcp_set_accept_cb,
    .set_listener_close_cb     = xcomm_async_tcp_set_listener_close_cb,
    .close_listener            = xcomm_async_tcp_close_listener,

    .set_recv_cb               = xcomm_async_tcp_set_recv_cb,
    .set_send_completed_cb     = xcomm_async_tcp_set_send_completed_cb,
    .set_heartbeat_cb          = xcomm_async_tcp_set_heartbeat_cb,
    .set_connection_close_cb   = xcomm_async_tcp_set_connection_close_cb,
    .close_connection          = xcomm_async_tcp_close_connection,
    .send                      = xcomm_async_tcp_send,
    .set_sendtimeo             = xcomm_async_tcp_set_sendtimeo,
    .set_recvtimeo             = xcomm_async_tcp_set_recvtimeo,
    .set_heartbeat_interval    = xcomm_async_tcp_set_heartbeat_interval,
    .set_packetizer            = xcomm_async_tcp_set_packetizer,
};
