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

#include <stdint.h>

#define xcomm_bswap(x)                               \
    _Generic((x),                                    \
        uint16_t: xcomm_bswap_u16,                   \
        uint32_t: xcomm_bswap_u32,                   \
        uint64_t: xcomm_bswap_u64,                   \
        int16_t:  xcomm_bswap_i16,                   \
        int32_t:  xcomm_bswap_i32,                   \
        int64_t:  xcomm_bswap_i64,                   \
        float:    xcomm_bswap_f32,                   \
        double:   xcomm_bswap_f64                    \
    )(x)

/* used by internal */
extern uint16_t xcomm_bswap_u16(uint16_t val);
extern uint32_t xcomm_bswap_u32(uint32_t val);
extern uint64_t xcomm_bswap_u64(uint64_t val);
extern int16_t  xcomm_bswap_i16(int16_t val);
extern int32_t  xcomm_bswap_i32(int32_t val);
extern int64_t  xcomm_bswap_i64(int64_t val);
extern float    xcomm_bswap_f32(float val);
extern double   xcomm_bswap_f64(double val);