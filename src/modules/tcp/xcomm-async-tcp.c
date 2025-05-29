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

#include "xcomm-async-tcp.h"

void xcomm_async_tcp_dial(
    const char* restrict host,
    const char* restrict port,
    void (*on_connect)(xcomm_tcp_channel_t* channel, void* userdata),
    void* userdata) {

}

void xcomm_async_tcp_listen(
    const char* restrict host,
    const char* restrict port,
    void (*on_accept)(xcomm_tcp_channel_t* channel, void* userdata),
    void* userdata) {

}

void xcomm_async_tcp_set_recv_cb(
    xcomm_tcp_channel_t* channel,
    void (*on_recv)(
        xcomm_tcp_channel_t* channel, void* buf, size_t len, void* userdata),
    void* userdata) {

}

void xcomm_async_tcp_set_send_cb(
    xcomm_tcp_channel_t* channel,
    void (*on_send_complete)(xcomm_tcp_channel_t* channel, void* userdata),
    void* userdata) {

}

void xcomm_async_tcp_set_close_cb(
    xcomm_tcp_channel_t* channel,
    void (*on_close)(xcomm_tcp_channel_t* channel, void* userdata),
    void* userdata) {

}

void xcomm_async_tcp_set_heartbeat_cb(
    xcomm_tcp_channel_t* channel,
    void (*on_heartbeat)(xcomm_tcp_channel_t* channel, void* userdata),
    void* userdata) {

}

void xcomm_async_tcp_send(xcomm_tcp_channel_t* channel, void* buf, size_t len) {

}

void xcomm_async_tcp_close(xcomm_tcp_channel_t* channel) {

}

void xcomm_async_tcp_set_conntimeo(xcomm_tcp_channel_t* channel, int timeout_ms) {

}

void xcomm_async_tcp_set_sendtimeo(
    xcomm_tcp_channel_t* channel, int timeout_ms) {

}

void xcomm_async_tcp_set_recvtimeo(
    xcomm_tcp_channel_t* channel, int timeout_ms) {

}

void xcomm_async_tcp_set_heartbeat_interval(
    xcomm_tcp_channel_t* channel, int interval_ms) {

}

void xcomm_async_tcp_set_packetizer(xcomm_tcp_packetizer_t* packetizer) {

}
