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

#include "xcomm-uart.h"
#include "xcomm-serial.h"
#include "xcomm-logger.h"

void xcomm_serial_close(xcomm_serial_t* serial) {
    xcomm_logi("%s enter.\n", __FUNCTION__);

    xcomm_uart_t* uartptr = serial->opaque;
    xcomm_uart_t  uartobj = *uartptr;
    xcomm_uart_close(uartobj);

    free(serial->opaque);
    free(serial);

    xcomm_logi("%s leave.\n", __FUNCTION__);
}

xcomm_serial_t* xcomm_serial_open(xcomm_serial_config_t* config) {
    xcomm_logi("%s enter.\n", __FUNCTION__);

    xcomm_uart_config_t* ucp = (void*)config;
    xcomm_serial_t* serial   = malloc(sizeof(xcomm_serial_t));
    if (!serial) {
        xcomm_loge("no memory.\n");
        return NULL;
    }
    serial->opaque = malloc(sizeof(xcomm_uart_t));
    if (!serial->opaque) {
        xcomm_loge("no memory.\n");
        free(serial);
        return NULL;
    }
    xcomm_uart_t uart = xcomm_uart_open(ucp);
    memcpy(serial->opaque, &uart, sizeof(xcomm_uart_t));

    xcomm_logi("%s leave.\n", __FUNCTION__);
    return serial;
}

int xcomm_serial_read(xcomm_serial_t* serial, uint8_t* buf, int len) {
    xcomm_uart_t* uartptr = serial->opaque;
    xcomm_uart_t  uartobj = *uartptr;
    return xcomm_uart_read(uartobj, buf, len);
}

int xcomm_serial_write(xcomm_serial_t* serial, uint8_t* buf, int len) {
    xcomm_uart_t* uartptr = serial->opaque;
    xcomm_uart_t  uartobj = *uartptr;
    return xcomm_uart_write(uartobj, buf, len);
}
