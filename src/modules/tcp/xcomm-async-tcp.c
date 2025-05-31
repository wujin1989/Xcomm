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
    const char* restrict   host,
    const char* restrict   port,
    xcomm_tcp_connect_cb_t connect_cb,
    void*                  userdata) {

}

void xcomm_async_tcp_listen(
    const char* restrict  host,
    const char* restrict  port,
    xcomm_tcp_listen_cb_t listen_cb,
    void*                 userdata) {

}

void xcomm_async_tcp_set_accept_cb(
    xcomm_tcp_listener_t* listener,
    xcomm_tcp_accept_cb_t accept_cb,
    void*                 userdata) {

}

void xcomm_async_tcp_set_destroy_listener_cb(
    xcomm_tcp_listener_t*           listener,
    xcomm_tcp_destroy_listener_cb_t destroy_listener_cb,
    void*                           userdata) {

}

void xcomm_async_tcp_destroy_listener(xcomm_tcp_listener_t* listener) {

}

void xcomm_async_tcp_set_recv_cb(
    xcomm_tcp_connection_t* conn, xcomm_tcp_recv_cb_t recv_cb, void* userdata) {

}

void xcomm_async_tcp_set_send_completed_cb(
    xcomm_tcp_connection_t*       conn,
    xcomm_tcp_send_completed_cb_t send_completed_cb,
    void*                         userdata) {

}

void xcomm_async_tcp_set_destroy_connection_cb(
    xcomm_tcp_connection_t*           conn,
    xcomm_tcp_destroy_connection_cb_t destroy_connection_cb,
    void*                             userdata) {

}

void xcomm_async_tcp_set_heartbeat_cb(
    xcomm_tcp_connection_t*  conn,
    xcomm_tcp_heartbeat_cb_t heartbeat_cb,
    void*                    userdata) {

}

void xcomm_async_tcp_send(xcomm_tcp_connection_t* conn, void* buf, size_t len) {

}

void xcomm_async_tcp_close(xcomm_tcp_channel_t* channel) {

}

void xcomm_async_tcp_set_conntimeo(
    xcomm_tcp_connection_t* conn, int timeout_ms) {

}

void xcomm_async_tcp_set_sendtimeo(
    xcomm_tcp_connection_t* conn, int timeout_ms) {

}

void xcomm_async_tcp_set_recvtimeo(
    xcomm_tcp_connection_t* conn, int timeout_ms) {

}

void xcomm_async_tcp_set_heartbeat_interval(
    xcomm_tcp_connection_t* conn, int interval_ms) {

}

void xcomm_async_tcp_set_packetizer(
    xcomm_tcp_connection_t* conn, xcomm_tcp_packetizer_t* packetizer) {

}
