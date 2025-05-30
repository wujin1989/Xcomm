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

#include "platform/platform-types.h"

typedef enum xcomm_endian_e         xcomm_endian_t;
typedef enum xcomm_time_precision_e xcomm_time_precision_t;

enum xcomm_endian_e {
    XCOMM_ENDIAN_BE = 0,
    XCOMM_ENDIAN_LE = 1,
};

enum xcomm_time_precision_e {
    XCOMM_TIME_PRECISION_SEC,
    XCOMM_TIME_PRECISION_MSEC,
    XCOMM_TIME_PRECISION_USEC,
    XCOMM_TIME_PRECISION_NSEC,
};

extern int            xcomm_utils_getprng(int min, int max);
extern uint64_t       xcomm_utils_getnow(xcomm_time_precision_t precision);
extern xcomm_endian_t xcomm_utils_getendian(void);

