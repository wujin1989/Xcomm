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

#include "xcomm-serial.h"
#include "xcomm-logger.h"
#include "platform/platform-uart.h"

static const platform_uart_baudrate_t baudrate_map[] = {
    [XCOMM_SERIAL_BAUDRATE_9600]   = PLATFORM_UART_BAUDRATE_9600,
    [XCOMM_SERIAL_BAUDRATE_19200]  = PLATFORM_UART_BAUDRATE_19200,
    [XCOMM_SERIAL_BAUDRATE_38400]  = PLATFORM_UART_BAUDRATE_38400,
    [XCOMM_SERIAL_BAUDRATE_57600]  = PLATFORM_UART_BAUDRATE_57600,
    [XCOMM_SERIAL_BAUDRATE_115200] = PLATFORM_UART_BAUDRATE_115200,
};

static const platform_uart_parity_t parity_map[] = {
    [XCOMM_SERIAL_PARITY_NO]   = PLATFORM_UART_PARITY_NO,
    [XCOMM_SERIAL_PARITY_ODD]  = PLATFORM_UART_PARITY_ODD,
    [XCOMM_SERIAL_PARITY_EVEN] = PLATFORM_UART_PARITY_EVEN,
};

static const platform_uart_databits_t databits_map[] = {
    [XCOMM_SERIAL_DATABITS_CS7] = PLATFORM_UART_DATABITS_CS7,
    [XCOMM_SERIAL_DATABITS_CS8] = PLATFORM_UART_DATABITS_CS8,
};

static const platform_uart_stopbits_t stopbits_map[] = {
    [XCOMM_SERIAL_STOPBITS_ONE] = PLATFORM_UART_STOPBITS_ONE,
    [XCOMM_SERIAL_STOPBITS_TWO] = PLATFORM_UART_STOPBITS_TWO,
};

void xcomm_serial_close(xcomm_serial_t* serial) {
    xcomm_logi("%s enter.\n", __FUNCTION__);

    platform_uart_t* uartptr = serial->opaque;
    platform_uart_t  uartobj = *uartptr;
    platform_uart_close(uartobj);

    free(serial->opaque);
    free(serial);

    xcomm_logi("%s leave.\n", __FUNCTION__);
}

xcomm_serial_t* xcomm_serial_open(xcomm_serial_config_t* config) {
    xcomm_logi("%s enter.\n", __FUNCTION__);

    xcomm_serial_t* serial = malloc(sizeof(xcomm_serial_t));
    if (!serial) {
        xcomm_loge("no memory.\n");
        return NULL;
    }
    serial->opaque = malloc(sizeof(platform_uart_t));
    if (!serial->opaque) {
        xcomm_loge("no memory.\n");
        free(serial);
        return NULL;
    }
    platform_uart_config_t uconfig = {
        .device   = config->device,
        .baudrate = baudrate_map[config->baudrate],
        .parity   = parity_map[config->parity],
        .databits = databits_map[config->databits],
        .stopbits = stopbits_map[config->stopbits],
    };
    platform_uart_t uartobj = platform_uart_open(&uconfig);
    if (uartobj != PLATFORM_UA_ERROR_INVALID_UART) {
        memcpy(serial->opaque, &uartobj, sizeof(platform_uart_t));
    } else {
        xcomm_loge("open serial failed.\n");
        free(serial->opaque);
        free(serial);
        return NULL;
    }
    xcomm_logi("%s leave.\n", __FUNCTION__);
    return serial;
}

int xcomm_serial_read(xcomm_serial_t* serial, uint8_t* buf, int len) {
    platform_uart_t* uartptr = serial->opaque;
    platform_uart_t  uartobj = *uartptr;

    int ret = platform_uart_read(uartobj, buf, len);
    if (ret == PLATFORM_UA_ERROR_UART_ERROR) {
        return -1;
    }
    return ret;
}

int xcomm_serial_write(xcomm_serial_t* serial, uint8_t* buf, int len) {
    platform_uart_t* uartptr = serial->opaque;
    platform_uart_t  uartobj = *uartptr;
    
    int ret = platform_uart_write(uartobj, buf, len);
    if (ret == PLATFORM_UA_ERROR_UART_ERROR) {
        return -1;
    }
    return ret;
}
