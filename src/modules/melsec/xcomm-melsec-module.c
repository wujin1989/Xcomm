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

#include "xcomm-melsec-1c.h"
#include "xcomm/xcomm-melsec-module.h"

xcomm_melsec_1c_module_t xcomm_melsec_1c = {
    .name         = "Xcomm Mitsubishi Melsec 1C Module",
    .dial         = xcomm_melsec_1c_dial,
    .close        = xcomm_melsec_1c_close,
    .load_bool    = xcomm_melsec_1c_load_bool,
    .load_int8    = xcomm_melsec_1c_load_int8,
    .load_int16   = xcomm_melsec_1c_load_int16,
    .load_int32   = xcomm_melsec_1c_load_int32,
    .load_int64   = xcomm_melsec_1c_load_int64,
    .load_uint8   = xcomm_melsec_1c_load_uint8,
    .load_uint16  = xcomm_melsec_1c_load_uint16,
    .load_uint32  = xcomm_melsec_1c_load_uint32,
    .load_uint64  = xcomm_melsec_1c_load_uint64,
    .load_float   = xcomm_melsec_1c_load_float,
    .load_double  = xcomm_melsec_1c_load_double,
    .load_string  = xcomm_melsec_1c_load_string,
    .store_bool   = xcomm_melsec_1c_store_bool,
    .store_int8   = xcomm_melsec_1c_store_int8,
    .store_int16  = xcomm_melsec_1c_store_int16,
    .store_int32  = xcomm_melsec_1c_store_int32,
    .store_int64  = xcomm_melsec_1c_store_int64,
    .store_uint8  = xcomm_melsec_1c_store_uint8,
    .store_uint16 = xcomm_melsec_1c_store_uint16,
    .store_uint32 = xcomm_melsec_1c_store_uint32,
    .store_uint64 = xcomm_melsec_1c_store_uint64,
    .store_float  = xcomm_melsec_1c_store_float,
    .store_double = xcomm_melsec_1c_store_double,
    .store_string = xcomm_melsec_1c_store_string,
};