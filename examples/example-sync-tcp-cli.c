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

#include <stdio.h>
#include <string.h>

#include "xcomm.h"

int main(void) {
    char sbuf[64] = "ping";
    char rbuf[64] = {0};
    xcomm_socket_t* socket = sync_tcp_module.xcomm_dial("127.0.0.1", "1234");
    
    sync_tcp_module.xcomm_send(socket, sbuf, sizeof(sbuf));
    printf("cli send %s to srv.\n", sbuf);

    sync_tcp_module.xcomm_recv(socket, rbuf, sizeof(rbuf));
    printf("cli recv %s from srv.\n", rbuf);

    sync_tcp_module.xcomm_close(socket);
	return 0;
}