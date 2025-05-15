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

#include "xcomm/xcomm-melsec-module.h"
#include "xcomm/xcomm-serial-module.h"

extern void xcomm_melsec_1c_close(xcomm_melsec_device_t* device);
extern xcomm_melsec_device_t* xcomm_melsec_1c_dial(xcomm_serial_config_t* config, const char* restrict stn_no, const char* restrict plc_no);

extern int xcomm_melsec_1c_load_bool(xcomm_melsec_device_t* device, const char* restrict addr, bool* dst);
extern int xcomm_melsec_1c_load_int8(xcomm_melsec_device_t* device, const char* restrict addr, int8_t* dst);
extern int xcomm_melsec_1c_load_uint8(xcomm_melsec_device_t* device, const char* restrict addr, uint8_t* dst);
extern int xcomm_melsec_1c_load_int16(xcomm_melsec_device_t* device, const char* restrict addr, int16_t* dst);
extern int xcomm_melsec_1c_load_uint16(xcomm_melsec_device_t* device, const char* restrict addr, uint16_t* dst);
extern int xcomm_melsec_1c_load_int32(xcomm_melsec_device_t* device, const char* restrict addr, int32_t* dst);
extern int xcomm_melsec_1c_load_uint32(xcomm_melsec_device_t* device, const char* restrict addr, uint32_t* dst);
extern int xcomm_melsec_1c_load_int64(xcomm_melsec_device_t* device, const char* restrict addr, int64_t* dst);
extern int xcomm_melsec_1c_load_uint64(xcomm_melsec_device_t* device, const char* restrict addr, uint64_t* dst);
extern int xcomm_melsec_1c_load_float(xcomm_melsec_device_t* device, const char* restrict addr, float* dst);
extern int xcomm_melsec_1c_load_double(xcomm_melsec_device_t* device, const char* restrict addr, double* dst);
extern int xcomm_melsec_1c_load_string(xcomm_melsec_device_t* device, const char* restrict addr, char* dst, int dstlen);

extern int xcomm_melsec_1c_store_bool(xcomm_melsec_device_t* device, const char* restrict addr, bool src);
extern int xcomm_melsec_1c_store_int8(xcomm_melsec_device_t* device, const char* restrict addr, int8_t src);
extern int xcomm_melsec_1c_store_uint8(xcomm_melsec_device_t* device, const char* restrict addr, uint8_t src);
extern int xcomm_melsec_1c_store_int16(xcomm_melsec_device_t* device, const char* restrict addr, int16_t src);
extern int xcomm_melsec_1c_store_uint16(xcomm_melsec_device_t* device, const char* restrict addr, uint16_t src);
extern int xcomm_melsec_1c_store_int32(xcomm_melsec_device_t* device, const char* restrict addr, int32_t src);
extern int xcomm_melsec_1c_store_uint32(xcomm_melsec_device_t* device, const char* restrict addr, uint32_t src);
extern int xcomm_melsec_1c_store_int64(xcomm_melsec_device_t* device, const char* restrict addr, int64_t src);
extern int xcomm_melsec_1c_store_uint64(xcomm_melsec_device_t* device, const char* restrict addr, uint64_t src);
extern int xcomm_melsec_1c_store_float(xcomm_melsec_device_t* device, const char* restrict addr, float src);
extern int xcomm_melsec_1c_store_double(xcomm_melsec_device_t* device, const char* restrict addr, double src);
extern int xcomm_melsec_1c_store_string(xcomm_melsec_device_t* device, const char* restrict addr, char* src, int slen);
