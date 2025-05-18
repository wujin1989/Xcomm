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
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "xcomm.h"

int main(void) {
    xcomm_dumper_config_t dumper_config = {
        .filename = NULL, 
        .mode     = XCOMM_DUMPER_MODE_FILE,
        .level    = XCOMM_DUMPER_LEVEL_DEBUG
    };
    xcomm_dumper.init(&dumper_config);

    xcomm_serial_config_t config = {
        .device   = "COM2",
        .baudrate = XCOMM_SERIAL_BAUDRATE_9600,
        .parity   = XCOMM_SERIAL_PARITY_NO,
        .databits = XCOMM_SERIAL_DATABITS_CS8,
        .stopbits = XCOMM_SERIAL_STOPBITS_ONE,
        .timeout_ms = 0,
    };
    xcomm_melsec_device_t* device = 
        xcomm_melsec_1c.dial(&config, NULL, NULL);

   /* float wf = 0.75f;
    xcomm_melsec_1c.store_float(device, "D0000", wf);

    float rf;
    xcomm_melsec_1c.load_float(device, "D0000", &rf);
    assert(rf == wf);

    char wstr[5] = "12AB";
    xcomm_melsec_1c.store_string(device, "D0000", wstr, sizeof(wstr));

    char rstr[5];
    xcomm_melsec_1c.load_string(device, "D0000", rstr, sizeof(rstr));
    assert(strcmp(rstr, wstr) == 0);*/

    float rf;
    xcomm_melsec_1c.load_float(device, "D0000", &rf);

    printf("%f\n", rf);

    xcomm_melsec_1c.close(device);
    return 0;
}