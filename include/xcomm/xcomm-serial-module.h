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

_Pragma("once")

#include <stdint.h>
#include <stddef.h>

typedef struct xcomm_serial_module_s xcomm_serial_module_t;
typedef struct xcomm_serial_config_s xcomm_serial_config_t;
typedef enum xcomm_serial_baudrate_e xcomm_serial_baudrate_t;
typedef enum xcomm_serial_parity_e   xcomm_serial_parity_t;
typedef enum xcomm_serial_databits_e xcomm_serial_databits_t;
typedef enum xcomm_serial_stopbits_e xcomm_serial_stopbits_t;
typedef struct xcomm_serial_s        xcomm_serial_t;

struct xcomm_serial_s {
    void* opaque;
};

enum xcomm_serial_baudrate_e {
    XCOMM_SERIAL_BAUDRATE_9600,
    XCOMM_SERIAL_BAUDRATE_19200,
    XCOMM_SERIAL_BAUDRATE_38400,
    XCOMM_SERIAL_BAUDRATE_57600,
    XCOMM_SERIAL_BAUDRATE_115200,
};

enum xcomm_serial_parity_e {
    XCOMM_SERIAL_PARITY_NO,
    XCOMM_SERIAL_PARITY_ODD,
    XCOMM_SERIAL_PARITY_EVEN,
};

enum xcomm_serial_databits_e {
    XCOMM_SERIAL_DATABITS_CS7,
    XCOMM_SERIAL_DATABITS_CS8,
};

enum xcomm_serial_stopbits_e {
    XCOMM_SERIAL_STOPBITS_ONE,
    XCOMM_SERIAL_STOPBITS_TWO,
};

struct xcomm_serial_config_s {
    const char* restrict    device;
    xcomm_serial_baudrate_t baudrate;
    xcomm_serial_parity_t   parity;
    xcomm_serial_databits_t databits;
    xcomm_serial_stopbits_t stopbits;
    size_t                  timeout_ms;
};

struct xcomm_serial_module_s {
    const char* restrict name;

    xcomm_serial_t* (*dial)(xcomm_serial_config_t* config);
    void (*close)(xcomm_serial_t* serial);
    int  (*recv)(xcomm_serial_t* serial, uint8_t* buf, int len);
    int  (*send)(xcomm_serial_t* serial, uint8_t* buf, int len);
};

extern xcomm_serial_module_t xcomm_serial;