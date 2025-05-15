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
#include <stdbool.h>

#include "xcomm-serial-module.h"

typedef struct xcomm_melsec_device_s      xcomm_melsec_device_t;
typedef struct xcomm_melsec_1c_module_s   xcomm_melsec_1c_module_t;
typedef struct xcomm_melsec_1e_module_s   xcomm_melsec_1e_module_t;
typedef struct xcomm_melsec_3c_module_s   xcomm_melsec_3c_module_t;
typedef struct xcomm_melsec_3e_module_s   xcomm_melsec_3e_module_t;

struct xcomm_melsec_device_s {
    void* opaque;
};

struct xcomm_melsec_1c_module_s {
    const char* restrict name;

    void (*close)(xcomm_melsec_device_t* device);
    xcomm_melsec_device_t* (*dial)(xcomm_serial_config_t* config, const char* restrict stn_no, const char* restrict plc_no);

    int (*load_bool)(xcomm_melsec_device_t* device, const char* restrict addr, bool* dst);
    int (*load_int16)(xcomm_melsec_device_t* device, const char* restrict addr, int16_t* dst);
    int (*load_int32)(xcomm_melsec_device_t* device, const char* restrict addr, int32_t* dst);
    int (*load_int64)(xcomm_melsec_device_t* device, const char* restrict addr, int64_t* dst);
    int (*load_uint16)(xcomm_melsec_device_t* device, const char* restrict addr, uint16_t* dst);
    int (*load_uint32)(xcomm_melsec_device_t* device, const char* restrict addr, uint32_t* dst);
    int (*load_uint64)(xcomm_melsec_device_t* device, const char* restrict addr, uint64_t* dst);
    int (*load_float)(xcomm_melsec_device_t* device, const char* restrict addr, float* dst);
    int (*load_double)(xcomm_melsec_device_t* device, const char* restrict addr, double* dst);
    int (*load_string)(xcomm_melsec_device_t* device, const char* restrict addr, char* dst, int dlen);

    int (*store_bool)(xcomm_melsec_device_t* device, const char* restrict addr, bool src);
    int (*store_int16)(xcomm_melsec_device_t* device, const char* restrict addr, int16_t src);
    int (*store_int32)(xcomm_melsec_device_t* device, const char* restrict addr, int32_t src);
    int (*store_int64)(xcomm_melsec_device_t* device, const char* restrict addr, int64_t src);
    int (*store_uint16)(xcomm_melsec_device_t* device, const char* restrict addr, uint16_t src);
    int (*store_uint32)(xcomm_melsec_device_t* device, const char* restrict addr, uint32_t src);
    int (*store_uint64)(xcomm_melsec_device_t* device, const char* restrict addr, uint64_t src);
    int (*store_float)(xcomm_melsec_device_t* device, const char* restrict addr, float src);
    int (*store_double)(xcomm_melsec_device_t* device, const char* restrict addr, double src);
    int (*store_string)(xcomm_melsec_device_t* device, const char* restrict addr, char* src, int slen);
};

struct xcomm_melsec_1e_module_s {
    const char* restrict name;
};

struct xcomm_melsec_3c_module_s {
    const char* restrict name;
};

struct xcomm_melsec_3e_module_s {
    const char* restrict name;
};

extern xcomm_melsec_1c_module_t xcomm_melsec_1c;
extern xcomm_melsec_1e_module_t xcomm_melsec_1e;
extern xcomm_melsec_3c_module_t xcomm_melsec_3c;
extern xcomm_melsec_3e_module_t xcomm_melsec_3e;