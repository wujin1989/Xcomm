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
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "xcomm-melsec-common.h"

static const char bit_char_table[] = {'0', '1'};
static const char hex_char_table[] = "0123456789ABCDEF";

void xcomm_melsec_bool_to_bit_chars(
    bool value, char bit_chars[XCOMM_MELSEC_1_BYTE]) {
    bit_chars[0] = value ? '1' : '0';
}

void xcomm_melsec_uint8_to_bit_chars(
    uint8_t value, char bit_chars[XCOMM_MELSEC_8_BYTE]) {
    for (int i = 0; i < XCOMM_MELSEC_8_BYTE; ++i) {
        bit_chars[i] = bit_char_table[(value & (0x80 >> i))];
    }
}

void xcomm_melsec_int8_to_bit_chars(
    int8_t value, char bit_chars[XCOMM_MELSEC_8_BYTE]) {
    xcomm_melsec_uint8_to_bit_chars((uint8_t)value, bit_chars);
}

void xcomm_melsec_uint16_to_bit_chars(
    uint16_t value, char bit_chars[XCOMM_MELSEC_16_BYTE]) {
    for (int i = 0; i < XCOMM_MELSEC_16_BYTE; ++i) {
        bit_chars[i] = bit_char_table[(value & ((uint16_t)0x8000 >> i))];
    }
}

void xcomm_melsec_int16_to_bit_chars(
    int16_t value, char bit_chars[XCOMM_MELSEC_16_BYTE]) {
    xcomm_melsec_uint16_to_bit_chars((uint16_t)value, bit_chars);
}

void xcomm_melsec_uint32_to_bit_chars(
    uint32_t value, char bit_chars[XCOMM_MELSEC_32_BYTE]) {
    for (int i = 0; i < XCOMM_MELSEC_32_BYTE; ++i) {
        bit_chars[i] = bit_char_table[(value & ((uint32_t)0x80000000 >> i))];
    }
}

void xcomm_melsec_int32_to_bit_chars(
    int32_t value, char bit_chars[XCOMM_MELSEC_32_BYTE]) {
    xcomm_melsec_uint32_to_bit_chars((uint32_t)value, bit_chars);
}

void xcomm_melsec_uint64_to_bit_chars(
    uint64_t value, char bit_chars[XCOMM_MELSEC_64_BYTE]) {
    for (int i = 0; i < XCOMM_MELSEC_64_BYTE; ++i) {
        bit_chars[i] =
            bit_char_table[(value & ((uint64_t)0x8000000000000000ULL >> i))];
    }
}

void xcomm_melsec_int64_to_bit_chars(
    int64_t value, char bit_chars[XCOMM_MELSEC_64_BYTE]) {
    xcomm_melsec_uint64_to_bit_chars((uint64_t)value, bit_chars);
}

void xcomm_melsec_float_to_bit_chars(
    float value, char bit_chars[XCOMM_MELSEC_32_BYTE]) {
    union {
        float    f;
        uint32_t u;
    } conv = {.f = value};
    xcomm_melsec_uint32_to_bit_chars(conv.u, bit_chars);
}

void xcomm_melsec_double_to_bit_chars(
    double value, char bit_chars[XCOMM_MELSEC_64_BYTE]) {
    union {
        double   d;
        uint64_t u;
    } conv = {.d = value};
    xcomm_melsec_uint64_to_bit_chars(conv.u, bit_chars);
}

void xcomm_melsec_uint8_to_nibble_chars(
    uint8_t value, uint8_t nibble_chars[XCOMM_MELSEC_2_BYTE]) {
    nibble_chars[0] = hex_char_table[(value >> 4) & 0x0F];
    nibble_chars[1] = hex_char_table[value        & 0x0F];
}

void xcomm_melsec_int8_to_nibble_chars(
    int8_t value, uint8_t nibble_chars[XCOMM_MELSEC_2_BYTE]) {
    xcomm_melsec_uint8_to_nibble_chars((uint8_t)value, nibble_chars);
}

void xcomm_melsec_uint16_to_nibble_chars(
    uint16_t value, uint8_t nibble_chars[XCOMM_MELSEC_4_BYTE]) {
    nibble_chars[0] = hex_char_table[(value >> 12) & 0x0F];
    nibble_chars[1] = hex_char_table[(value >> 8)  & 0x0F];
    nibble_chars[2] = hex_char_table[(value >> 4)  & 0x0F];
    nibble_chars[3] = hex_char_table[value         & 0x0F];
}

void xcomm_melsec_int16_to_nibble_chars(
    int16_t value, uint8_t nibble_chars[XCOMM_MELSEC_4_BYTE]) {
    xcomm_melsec_uint16_to_nibble_chars((uint16_t)value, nibble_chars);
}

void xcomm_melsec_uint32_to_nibble_chars(
    uint32_t value, uint8_t nibble_chars[XCOMM_MELSEC_8_BYTE]) {
    nibble_chars[0] = hex_char_table[(value >> 28) & 0x0F];
    nibble_chars[1] = hex_char_table[(value >> 24) & 0x0F];
    nibble_chars[2] = hex_char_table[(value >> 20) & 0x0F];
    nibble_chars[3] = hex_char_table[(value >> 16) & 0x0F];
    nibble_chars[4] = hex_char_table[(value >> 12) & 0x0F];
    nibble_chars[5] = hex_char_table[(value >> 8)  & 0x0F];
    nibble_chars[6] = hex_char_table[(value >> 4)  & 0x0F];
    nibble_chars[7] = hex_char_table[value         & 0x0F];
}

void xcomm_melsec_int32_to_nibble_chars(
    int32_t value, uint8_t nibble_chars[XCOMM_MELSEC_8_BYTE]) {
    xcomm_melsec_uint32_to_nibble_chars((uint32_t)value, nibble_chars);
}

void xcomm_melsec_float_to_nibble_chars(
    float value, uint8_t nibble_chars[XCOMM_MELSEC_8_BYTE]) {
    union {
        float    f;
        uint32_t u;
    } conv = {.f = value};
    xcomm_melsec_uint32_to_nibble_chars(conv.u, nibble_chars);
}

void xcomm_melsec_uint64_to_nibble_chars(
    uint64_t value, uint8_t nibble_chars[XCOMM_MELSEC_16_BYTE]) {
    nibble_chars[0]  = hex_char_table[(value >> 60) & 0x0F];
    nibble_chars[1]  = hex_char_table[(value >> 56) & 0x0F];
    nibble_chars[2]  = hex_char_table[(value >> 52) & 0x0F];
    nibble_chars[3]  = hex_char_table[(value >> 48) & 0x0F];
    nibble_chars[4]  = hex_char_table[(value >> 44) & 0x0F];
    nibble_chars[5]  = hex_char_table[(value >> 40) & 0x0F];
    nibble_chars[6]  = hex_char_table[(value >> 36) & 0x0F];
    nibble_chars[7]  = hex_char_table[(value >> 32) & 0x0F];
    nibble_chars[8]  = hex_char_table[(value >> 28) & 0x0F];
    nibble_chars[9]  = hex_char_table[(value >> 24) & 0x0F];
    nibble_chars[10] = hex_char_table[(value >> 20) & 0x0F];
    nibble_chars[11] = hex_char_table[(value >> 16) & 0x0F];
    nibble_chars[12] = hex_char_table[(value >> 12) & 0x0F];
    nibble_chars[13] = hex_char_table[(value >> 8)  & 0x0F];
    nibble_chars[14] = hex_char_table[(value >> 4)  & 0x0F];
    nibble_chars[15] = hex_char_table[value         & 0x0F];
}

void xcomm_melsec_int64_to_nibble_chars(
    int64_t value, uint8_t nibble_chars[XCOMM_MELSEC_16_BYTE]) {
    xcomm_melsec_uint64_to_nibble_chars((uint64_t)value, nibble_chars);
}

void xcomm_melsec_double_to_nibble_chars(
    double value, uint8_t nibble_chars[XCOMM_MELSEC_16_BYTE]) {
    union {
        double   d;
        uint64_t u;
    } conv = {.d = value};
    xcomm_melsec_uint64_to_nibble_chars(conv.u, nibble_chars);
}

void xcomm_melsec_value_to_bytes(
    xcomm_melsec_operate_t        op_code,
    xcomm_melsec_value_t*         val,
    xcomm_melsec_byte_sequence_t* bytes) {
    if (op_code == XCOMM_MELSEC_B_OP) {
        switch (val->type) {
        case XCOMM_MELSEC_BOOL: {
            char bit_chars[XCOMM_MELSEC_1_BYTE];
            xcomm_melsec_value_to_bit_chars(val->b, bit_chars);
            memcpy(bytes->data, bit_chars, bytes->size);
            break;
        }
        case XCOMM_MELSEC_INT16: {
            char bit_chars[XCOMM_MELSEC_16_BYTE];
            xcomm_melsec_value_to_bit_chars(val->i16, bit_chars);
            memcpy(bytes->data, bit_chars, bytes->size);
            break;
        }
        case XCOMM_MELSEC_UINT16: {
            char bit_chars[XCOMM_MELSEC_16_BYTE];
            xcomm_melsec_value_to_bit_chars(val->u16, bit_chars);
            memcpy(bytes->data, bit_chars, bytes->size);
            break;
        }
        case XCOMM_MELSEC_INT32: {
            char bit_chars[XCOMM_MELSEC_32_BYTE];
            xcomm_melsec_value_to_bit_chars(val->i32, bit_chars);
            memcpy(bytes->data, bit_chars, bytes->size);
            break;
        }
        case XCOMM_MELSEC_UINT32: {
            char bit_chars[XCOMM_MELSEC_32_BYTE];
            xcomm_melsec_value_to_bit_chars(val->i32, bit_chars);
            memcpy(bytes->data, bit_chars, bytes->size);
            break;
        }
        case XCOMM_MELSEC_FLOAT: {
            char bit_chars[XCOMM_MELSEC_32_BYTE];
            xcomm_melsec_value_to_bit_chars(val->f32, bit_chars);
            memcpy(bytes->data, bit_chars, bytes->size);
            break;
        }
        case XCOMM_MELSEC_INT64: {
            char bit_chars[XCOMM_MELSEC_64_BYTE];
            xcomm_melsec_value_to_bit_chars(val->i64, bit_chars);
            memcpy(bytes->data, bit_chars, bytes->size);
            break;
        }
        case XCOMM_MELSEC_UINT64: {
            char bit_chars[XCOMM_MELSEC_64_BYTE];
            xcomm_melsec_value_to_bit_chars(val->u64, bit_chars);
            memcpy(bytes->data, bit_chars, bytes->size);
            break;
        }
        case XCOMM_MELSEC_DOUBLE: {
            char bit_chars[XCOMM_MELSEC_64_BYTE];
            xcomm_melsec_value_to_bit_chars(val->f64, bit_chars);
            memcpy(bytes->data, bit_chars, bytes->size);
            break;
        }
        default:
            break;
        }
    }
    if (op_code == XCOMM_MELSEC_W_OP) {
        switch (val->type) {
        case XCOMM_MELSEC_INT16: {
            char nibble_chars[XCOMM_MELSEC_4_BYTE];
            xcomm_melsec_value_to_nibble_chars(val->i16, nibble_chars);
            memcpy(bytes->data, nibble_chars, bytes->size);
            break;
        }
        case XCOMM_MELSEC_UINT16: {
            char nibble_chars[XCOMM_MELSEC_4_BYTE];
            xcomm_melsec_value_to_nibble_chars(val->u16, nibble_chars);
            memcpy(bytes->data, nibble_chars, bytes->size);
            break;
        }
        case XCOMM_MELSEC_INT32: {
            char nibble_chars[XCOMM_MELSEC_8_BYTE];
            xcomm_melsec_value_to_nibble_chars(val->i32, nibble_chars);
            memcpy(bytes->data, nibble_chars, bytes->size);
            break;
        }
        case XCOMM_MELSEC_UINT32: {
            char nibble_chars[XCOMM_MELSEC_8_BYTE];
            xcomm_melsec_value_to_nibble_chars(val->u32, nibble_chars);
            memcpy(bytes->data, nibble_chars, bytes->size);
            break;
        }
        case XCOMM_MELSEC_FLOAT: {
            char nibble_chars[XCOMM_MELSEC_8_BYTE];
            xcomm_melsec_value_to_nibble_chars(val->f32, nibble_chars);
            memcpy(bytes->data, nibble_chars, bytes->size);
            break;
        }
        case XCOMM_MELSEC_INT64: {
            char nibble_chars[XCOMM_MELSEC_16_BYTE];
            xcomm_melsec_value_to_nibble_chars(val->i64, nibble_chars);
            memcpy(bytes->data, nibble_chars, bytes->size);
            break;
        }
        case XCOMM_MELSEC_UINT64: {
            char nibble_chars[XCOMM_MELSEC_16_BYTE];
            xcomm_melsec_value_to_nibble_chars(val->u64, nibble_chars);
            memcpy(bytes->data, nibble_chars, bytes->size);
            break;
        }
        case XCOMM_MELSEC_DOUBLE: {
            char nibble_chars[XCOMM_MELSEC_16_BYTE];
            xcomm_melsec_value_to_nibble_chars(val->f64, nibble_chars);
            memcpy(bytes->data, nibble_chars, bytes->size);
            break;
        }
        case XCOMM_MELSEC_STRING:
            memcpy(bytes->data, val->str, bytes->size);
            break;
        default:
            break;
        }
    }
    return;
}

void xcomm_melsec_bytes_to_value(
    xcomm_melsec_operate_t        op_code,
    xcomm_melsec_byte_sequence_t* bytes,
    xcomm_melsec_value_t*         val) {
    char str[XCOMM_MELSEC_512_BYTE] = {0};
    memcpy(str, bytes->data, bytes->size);

    if (op_code == XCOMM_MELSEC_B_OP) {
        val->u64 = strtoll(str, NULL, 2);
    }
    if (op_code == XCOMM_MELSEC_W_OP) {
        val->u64 = strtoll(str, NULL, 16);
    }
}