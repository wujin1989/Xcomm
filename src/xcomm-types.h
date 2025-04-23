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

#include "platform/platform-types.h"

#define XCOMM_SO_ERROR_EAGAIN            PLATFORM_SO_ERROR_EAGAIN
#define XCOMM_SO_ERROR_EWOULDBLOCK       PLATFORM_SO_ERROR_EWOULDBLOCK
#define XCOMM_SO_ERROR_ECONNRESET        PLATFORM_SO_ERROR_ECONNRESET
#define XCOMM_SO_ERROR_ETIMEDOUT         PLATFORM_SO_ERROR_ETIMEDOUT
#define XCOMM_SO_ERROR_INVALID_SOCKET    PLATFORM_SO_ERROR_INVALID_SOCKET
#define XCOMM_SO_ERROR_SOCKET_ERROR      PLATFORM_SO_ERROR_SOCKET_ERROR

#define XCOMM_MAX_PROCESS_EVENTS         PLATFORM_MAX_PROCESS_EVENTS

typedef platform_tid_t       xcomm_tid_t;
typedef platform_pid_t       xcomm_pid_t;
typedef platform_pollfd_t    xcomm_pollfd_t;
typedef platform_sock_t      xcomm_sock_t;
typedef platform_pollevent_t xcomm_pollevent_t;
