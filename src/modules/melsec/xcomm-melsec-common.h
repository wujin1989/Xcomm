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
#include <stddef.h>
#include <stdbool.h>

#define XCOMM_MELSEC_CHARS_PER_B_POINT    1
#define XCOMM_MELSEC_CHARS_PER_W_POINT    4

typedef struct xcomm_melsec_byte_sequence_s xcomm_melsec_byte_sequence_t;
typedef enum xcomm_melsec_operate_e         xcomm_melsec_operate_t;
typedef struct xcomm_melsec_value_s         xcomm_melsec_value_t;
typedef enum xcomm_melsec_value_type_e      xcomm_melsec_value_type_t;

enum {
    XCOMM_MELSEC_1_BYTE    = 1,
    XCOMM_MELSEC_2_BYTE    = 2,
    XCOMM_MELSEC_3_BYTE    = 3,
    XCOMM_MELSEC_4_BYTE    = 4,
    XCOMM_MELSEC_5_BYTE    = 5,
    XCOMM_MELSEC_6_BYTE    = 6,
    XCOMM_MELSEC_7_BYTE    = 7,
    XCOMM_MELSEC_8_BYTE    = 8,
    XCOMM_MELSEC_16_BYTE   = 16,
    XCOMM_MELSEC_32_BYTE   = 32,
    XCOMM_MELSEC_64_BYTE   = 64,
    XCOMM_MELSEC_128_BYTE  = 128,
    XCOMM_MELSEC_256_BYTE  = 256,
    XCOMM_MELSEC_512_BYTE  = 512,
};

enum xcomm_melsec_operate_e{
    XCOMM_MELSEC_B_OP, 
    XCOMM_MELSEC_W_OP,
};

struct xcomm_melsec_byte_sequence_s {
    uint8_t* data;
    size_t   size;
};

enum xcomm_melsec_value_type_e {
    XCOMM_MELSEC_BOOL,
    XCOMM_MELSEC_INT16,
    XCOMM_MELSEC_UINT16,
    XCOMM_MELSEC_INT32,
    XCOMM_MELSEC_UINT32,
    XCOMM_MELSEC_INT64,
    XCOMM_MELSEC_UINT64,
    XCOMM_MELSEC_FLOAT,
    XCOMM_MELSEC_DOUBLE,
    XCOMM_MELSEC_STRING,
};

struct xcomm_melsec_value_s {
    xcomm_melsec_value_type_t type;
    union {
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
    } ;

};

extern void xcomm_melsec_value_to_bytes(xcomm_melsec_operate_t op_code, xcomm_melsec_value_t* val, xcomm_melsec_byte_sequence_t* bytes);
extern void xcomm_melsec_bytes_to_value(xcomm_melsec_operate_t op_code, xcomm_melsec_byte_sequence_t* bytes, xcomm_melsec_value_t* val);


/**
 * used by internal.
 */
extern void xcomm_melsec_bool_to_bit_chars(bool value, char bit_chars[XCOMM_MELSEC_1_BYTE]);
extern void xcomm_melsec_uint8_to_bit_chars(uint8_t value, char bit_chars[XCOMM_MELSEC_8_BYTE]);
extern void xcomm_melsec_int8_to_bit_chars(int8_t value, char bit_chars[XCOMM_MELSEC_8_BYTE]);
extern void xcomm_melsec_uint16_to_bit_chars(uint16_t value, char bit_chars[XCOMM_MELSEC_16_BYTE]);
extern void xcomm_melsec_int16_to_bit_chars(int16_t value, char bit_chars[XCOMM_MELSEC_16_BYTE]);
extern void xcomm_melsec_uint32_to_bit_chars(uint32_t value, char bit_chars[XCOMM_MELSEC_32_BYTE]);
extern void xcomm_melsec_int32_to_bit_chars(int32_t value, char bit_chars[XCOMM_MELSEC_32_BYTE]);
extern void xcomm_melsec_uint64_to_bit_chars(uint64_t value, char bit_chars[XCOMM_MELSEC_64_BYTE]);
extern void xcomm_melsec_int64_to_bit_chars(int64_t value, char bit_chars[XCOMM_MELSEC_64_BYTE]);
extern void xcomm_melsec_float_to_bit_chars(float value, char bit_chars[XCOMM_MELSEC_32_BYTE]);
extern void xcomm_melsec_double_to_bit_chars(double value, char bit_chars[XCOMM_MELSEC_64_BYTE]);

extern void xcomm_melsec_uint8_to_nibble_chars(uint8_t value, uint8_t nibble_chars[XCOMM_MELSEC_2_BYTE]);
extern void xcomm_melsec_int8_to_nibble_chars(int8_t value, uint8_t nibble_chars[XCOMM_MELSEC_2_BYTE]);
extern void xcomm_melsec_uint16_to_nibble_chars(uint16_t value, uint8_t nibble_chars[XCOMM_MELSEC_4_BYTE]);
extern void xcomm_melsec_int16_to_nibble_chars(int16_t value, uint8_t nibble_chars[XCOMM_MELSEC_4_BYTE]);
extern void xcomm_melsec_uint32_to_nibble_chars(uint32_t value, uint8_t nibble_chars[XCOMM_MELSEC_8_BYTE]);
extern void xcomm_melsec_int32_to_nibble_chars(int32_t value, uint8_t nibble_chars[XCOMM_MELSEC_8_BYTE]);
extern void xcomm_melsec_float_to_nibble_chars(float value, uint8_t nibble_chars[XCOMM_MELSEC_8_BYTE]);
extern void xcomm_melsec_uint64_to_nibble_chars(uint64_t value, uint8_t nibble_chars[XCOMM_MELSEC_16_BYTE]);
extern void xcomm_melsec_int64_to_nibble_chars(int64_t value, uint8_t nibble_chars[XCOMM_MELSEC_16_BYTE]);
extern void xcomm_melsec_double_to_nibble_chars(double value, uint8_t nibble_chars[XCOMM_MELSEC_16_BYTE]);
