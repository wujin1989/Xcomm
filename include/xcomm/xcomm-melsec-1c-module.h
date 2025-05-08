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

typedef struct xcomm_melsec_1c_module_s xcomm_melsec_1c_module_t;
typedef struct xcomm_melsec_1c_device_s xcomm_melsec_1c_device_t;

struct xcomm_melsec_1c_device_s {
    void* opaque;
};

struct xcomm_melsec_1c_module_s {
    const char* restrict name;
    
    xcomm_melsec_1c_device_t* (*xcomm_dial)(xcomm_serial_config_t* config, const char* restrict station_no, const char* restrict plc_no);
    void (*xcomm_close)(xcomm_melsec_1c_device_t* device);

    bool     (*xcomm_bool_load)(xcomm_melsec_1c_device_t* device, const char* restrict addr);
    int8_t   (*xcomm_int8_load)(xcomm_melsec_1c_device_t* device, const char* restrict addr);
    int16_t  (*xcomm_int16_load)(xcomm_melsec_1c_device_t* device, const char* restrict addr);
    int32_t  (*xcomm_int32_load)(xcomm_melsec_1c_device_t* device, const char* restrict addr);
    int64_t  (*xcomm_int64_load)(xcomm_melsec_1c_device_t* device, const char* restrict addr);
    uint8_t  (*xcomm_uint8_load)(xcomm_melsec_1c_device_t* device, const char* restrict addr);
    uint16_t (*xcomm_uint16_load)(xcomm_melsec_1c_device_t* device, const char* restrict addr);
    uint32_t (*xcomm_uint32_load)(xcomm_melsec_1c_device_t* device, const char* restrict addr);
    uint64_t (*xcomm_uint64_load)(xcomm_melsec_1c_device_t* device, const char* restrict addr);
    const char* (*xcomm_string_load)(xcomm_melsec_1c_device_t* device, const char* restrict addr);

    void (*xcomm_bool_store)(xcomm_melsec_1c_device_t* device, const char* restrict addr, bool val);
    void (*xcomm_int8_store)(xcomm_melsec_1c_device_t* device, const char* restrict addr, int8_t val);
    void (*xcomm_int16_store)(xcomm_melsec_1c_device_t* device, const char* restrict addr, int16_t val);
    void (*xcomm_int32_store)(xcomm_melsec_1c_device_t* device, const char* restrict addr, int32_t val);
    void (*xcomm_int64_store)(xcomm_melsec_1c_device_t* device, const char* restrict addr, int64_t val);
    void (*xcomm_uint8_store)(xcomm_melsec_1c_device_t* device, const char* restrict addr, uint8_t val);
    void (*xcomm_uint16_store)(xcomm_melsec_1c_device_t* device, const char* restrict addr, uint16_t val);
    void (*xcomm_uint32_store)(xcomm_melsec_1c_device_t* device, const char* restrict addr, uint32_t val);
    void (*xcomm_uint64_store)(xcomm_melsec_1c_device_t* device, const char* restrict addr, uint64_t val);
    void (*xcomm_string_store)(xcomm_melsec_1c_device_t* device, const char* restrict addr, const char* restrict val);
};

extern xcomm_melsec_1c_module_t xcomm_melsec_1c_module;