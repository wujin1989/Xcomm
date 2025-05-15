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

#include "xcomm-logger.h"

#define XCOMM_MELSEC_CHARS_PER_B_POINT    1
#define XCOMM_MELSEC_CHARS_PER_W_POINT    4
#define XCOMM_MELSEC_MAX_MESSAGE_SIZE     64
#define XCOMM_MELSEC_MAX_MESSAGE_STR_SIZE 256

typedef struct xcomm_melsec_bytes_s         xcomm_melsec_bytes_t;
typedef enum xcomm_melsec_operate_e         xcomm_melsec_operate_t;
typedef union xcomm_melsec_flexible_value_u xcomm_melsec_flexible_value_t;

enum {
    XCOMM_MELSEC_1_BYTE = 1,
    XCOMM_MELSEC_2_BYTE = 2,
    XCOMM_MELSEC_3_BYTE = 3,
    XCOMM_MELSEC_4_BYTE = 4,
    XCOMM_MELSEC_5_BYTE = 5,
    XCOMM_MELSEC_6_BYTE = 6,
    XCOMM_MELSEC_7_BYTE = 7,
    XCOMM_MELSEC_8_BYTE = 8,
};

enum xcomm_melsec_operate_e{
    XCOMM_MELSEC_B_OP, 
    XCOMM_MELSEC_W_OP,
};

struct xcomm_melsec_bytes_s {
    uint8_t* data;
    size_t   size;
};

union xcomm_melsec_flexible_value_u {
    bool     b;
    int16_t  i16;
    int32_t  i32;
    int64_t  i64;
    uint16_t u16;
    uint32_t u32;
    uint64_t u64;
    float    f32;
    double   f64;
    char*    str;
};

extern void xcomm_melsec_byte_to_ascii(uint8_t byte, uint8_t ascii[2]);
extern void xcomm_melsec_bytes_to_ascii_string(uint8_t* src, size_t slen, char* dst, size_t dlen, const char* delimiter);
extern void xcomm_melsec_bytes_to_hex_string(uint8_t* src, size_t slen, char* dst, size_t dlen, const char* delimiter);
extern void xcomm_melsec_ascii_checksum_calc(uint8_t* data, size_t len, uint8_t checksum[XCOMM_MELSEC_2_BYTE]);
extern void xcomm_melsec_binary_checksum_calc(uint8_t* data, size_t len, uint8_t* checksum);
