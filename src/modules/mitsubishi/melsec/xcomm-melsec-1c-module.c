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

#include <stdlib.h>
#include <string.h>

#include "xcomm-logger.h"

#include "internal-melsec-1c.h"
#include "xcomm/xcomm-serial-module.h"
#include "xcomm/xcomm-melsec-1c-module.h"

typedef struct internal_melsec_1c_device_s internal_melsec_1c_device_t;

struct internal_melsec_1c_device_s {
    char            station_no[INTERNAL_STATION_NO_LENGTH];
    char            plc_no[INTERNAL_PLC_NO_LENGTH];
    xcomm_serial_t* serialptr;
};

static xcomm_melsec_1c_device_t* _melsec_1c_dial(
    xcomm_serial_config_t* config,
    const char* restrict station_no,
    const char* restrict plc_no) {
    xcomm_logi("%s enter.\n", __FUNCTION__);

    xcomm_melsec_1c_device_t* devptr = malloc(sizeof(xcomm_melsec_1c_device_t));
    if (!devptr) {
        xcomm_loge("no memory.\n");
        return NULL;
    }
    devptr->opaque = malloc(sizeof(internal_melsec_1c_device_t));
    if (!devptr->opaque) {
        xcomm_loge("no memory.\n");
        free(devptr);
        return NULL;
    }
    internal_melsec_1c_device_t* internalptr = devptr->opaque;
    internalptr->serialptr = xcomm_serial_module.xcomm_dial(config);

    memcpy(internalptr->station_no, station_no, INTERNAL_STATION_NO_LENGTH);
    memcpy(internalptr->plc_no, plc_no, INTERNAL_PLC_NO_LENGTH);

    xcomm_logi("%s leave.\n", __FUNCTION__);
    return devptr;
}

static void _melsec_1c_close(xcomm_melsec_1c_device_t* device) {
    xcomm_logi("%s enter.\n", __FUNCTION__);

    internal_melsec_1c_device_t* internalptr = device->opaque;
    xcomm_serial_module.xcomm_close(internalptr->serialptr);

    free(device->opaque);
    free(device);

    xcomm_logi("%s leave.\n", __FUNCTION__);
}

static bool _melsec_1c_bool_load(
    xcomm_melsec_1c_device_t* device, const char* restrict addr) {
}

static int8_t _melsec_1c_int8_load(
    xcomm_melsec_1c_device_t* device, const char* restrict addr) {
}

static int16_t _melsec_1c_int16_load(
    xcomm_melsec_1c_device_t* device, const char* restrict addr) {
}

static int32_t _melsec_1c_int32_load(
    xcomm_melsec_1c_device_t* device, const char* restrict addr) {
}

static int64_t _melsec_1c_int64_load(
    xcomm_melsec_1c_device_t* device, const char* restrict addr) {
}

static uint8_t _melsec_1c_uint8_load(
    xcomm_melsec_1c_device_t* device, const char* restrict addr) {
}

static uint16_t _melsec_1c_uint16_load(
    xcomm_melsec_1c_device_t* device, const char* restrict addr) {
}

static uint32_t _melsec_1c_uint32_load(
    xcomm_melsec_1c_device_t* device, const char* restrict addr) {
}

static uint64_t _melsec_1c_uint64_load(
    xcomm_melsec_1c_device_t* device, const char* restrict addr) {
}

static void _melsec_1c_bool_store(
    xcomm_melsec_1c_device_t* device, const char* restrict addr, bool val) {
}

static void _melsec_1c_int8_store(
    xcomm_melsec_1c_device_t* device, const char* restrict addr, int8_t val) {
}

static void _melsec_1c_int16_store(
    xcomm_melsec_1c_device_t* device, const char* restrict addr, int16_t val) {
}

static void _melsec_1c_int32_store(
    xcomm_melsec_1c_device_t* device, const char* restrict addr, int32_t val) {
}

static void _melsec_1c_int64_store(
    xcomm_melsec_1c_device_t* device, const char* restrict addr, int64_t val) {
}

static void _melsec_1c_uint8_store(
    xcomm_melsec_1c_device_t* device, const char* restrict addr, uint8_t val) {
}

static void _melsec_1c_uint16_store(
    xcomm_melsec_1c_device_t* device, const char* restrict addr, uint16_t val) {
}

static void _melsec_1c_uint32_store(
    xcomm_melsec_1c_device_t* device, const char* restrict addr, uint32_t val) {
}

static void _melsec_1c_uint64_store(
    xcomm_melsec_1c_device_t* device, const char* restrict addr, uint64_t val) {
}

xcomm_melsec_1c_module_t xcomm_melsec_1c_module = {
    .name = "Xcomm mitsubishi melsec 1C module",

    .xcomm_dial = _melsec_1c_dial,
    .xcomm_close = _melsec_1c_close,

    .xcomm_bool_load = _melsec_1c_bool_load,
    .xcomm_int8_load = _melsec_1c_int8_load,
    .xcomm_int16_load = _melsec_1c_int16_load,
    .xcomm_int32_load = _melsec_1c_int32_load,
    .xcomm_int64_load = _melsec_1c_int64_load,
    .xcomm_uint8_load = _melsec_1c_uint8_load,
    .xcomm_uint16_load = _melsec_1c_uint16_load,
    .xcomm_uint32_load = _melsec_1c_uint32_load,
    .xcomm_uint64_load = _melsec_1c_uint64_load,

    .xcomm_bool_store = _melsec_1c_bool_store,
    .xcomm_int8_store = _melsec_1c_int8_store,
    .xcomm_int16_store = _melsec_1c_int16_store,
    .xcomm_int32_store = _melsec_1c_int32_store,
    .xcomm_int64_store = _melsec_1c_int64_store,
    .xcomm_uint8_store = _melsec_1c_uint8_store,
    .xcomm_uint16_store = _melsec_1c_uint16_store,
    .xcomm_uint32_store = _melsec_1c_uint32_store,
    .xcomm_uint64_store = _melsec_1c_uint64_store,
};