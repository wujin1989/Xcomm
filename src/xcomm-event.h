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

#include "xcomm-socket.h"

#define XCOMM_MAX_PROCESS_EVENTS PLATFORM_MAX_PROCESS_EVENTS

typedef platform_pollevent_t xcomm_pollevent_t;
typedef enum xcomm_event_e   xcomm_event_t;

enum xcomm_event_e {
    XCOMM_EVENT_RD = 1,
    XCOMM_EVENT_WR = 2,
};

extern void xcomm_event_add(xcomm_pollfd_t pfd, xcomm_sock_t sfd, int events, void* ud);
extern void xcomm_event_mod(xcomm_pollfd_t pfd, xcomm_sock_t sfd, int events, void* ud);
extern void xcomm_event_del(xcomm_pollfd_t pfd, xcomm_sock_t sfd);
extern int  xcomm_event_wait(xcomm_pollfd_t pfd, xcomm_pollevent_t* events, int timeout);