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

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "xcomm.h"

int main(void) {
    xcomm_dumper_config_t config = {
        .mode = XCOMM_DUMPER_MODE_FILE, .filename = NULL
    };
    xcomm_dumper_init(&config);
    xcomm_serial_config_t serial_config = {
        .device    = "COM3",
        .baudrate  = XCOMM_SERIAL_BAUDRATE_9600,
        .parity    = XCOMM_SERIAL_PARITY_NO,
        .databits  = XCOMM_SERIAL_DATABITS_CS8,
        .stopbits  = XCOMM_SERIAL_STOPBITS_ONE,
    };
    xcomm_serial_t* serial = serial_module.xcomm_serial_open(&serial_config);
    if (!serial) {
        return -1;
    }
    uint8_t buffer[64];
    while (true) {
        memset(buffer, 0, sizeof(buffer));
        serial_module.xcomm_serial_read(serial, buffer, strlen("hello world"));
        printf("read %s from serial.\n", buffer);
    }
    xcomm_dumper_destroy();
	return 0;
}