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
#include <stdlib.h>
#include <string.h>

#include "xcomm-melsec-1c.h"
#include "xcomm-melsec-common.h"

typedef struct melsec_1c_device_ctx_s melsec_1c_device_ctx_t;

struct melsec_1c_device_ctx_s {
    char stn_no[XCOMM_MELSEC_2_BYTE];
    char plc_no[XCOMM_MELSEC_2_BYTE];
    xcomm_serial_t* serial;
};

static const uint8_t default_stn_no[] = {0x30, 0x30};
static const uint8_t default_plc_no[] = {0x46, 0x46};

static const uint8_t enq = 0x05;
static const uint8_t stx = 0x02;
static const uint8_t etx = 0x03;
static const uint8_t nak = 0x15;
static const uint8_t timewait = 0x46;

static const uint8_t br_instruction[] = {0x45, 0x52};
static const uint8_t wr_instruction[] = {0x57, 0x52};
static const uint8_t bw_instruction[] = {0x42, 0x57};
static const uint8_t ww_instruction[] = {0x57, 0x57};

static inline xcomm_melsec_bytes_t _rd_request_marshalling(
    uint8_t* stn_no,
    uint8_t* plc_no,
    const char* restrict addr,
    xcomm_melsec_operate_t op_code,
    uint8_t                num_points) {
    xcomm_melsec_bytes_t result = {0};
    /**
     * | 1  |    2    |  2  |      2      |    1     |       5       |   2    |    2
     * |ENQ | STATION | PLC | INSTRUCTION | TIMEWAIT | START_ADDRESS | POINTS | CHECKSUM
     */
    result.size = XCOMM_MELSEC_1_BYTE + XCOMM_MELSEC_2_BYTE +
                  XCOMM_MELSEC_2_BYTE + XCOMM_MELSEC_2_BYTE +
                  XCOMM_MELSEC_1_BYTE + XCOMM_MELSEC_5_BYTE +
                  XCOMM_MELSEC_2_BYTE + XCOMM_MELSEC_2_BYTE;
    result.data = malloc(result.size);
    if (!result.data) {
        result.size = 0;
        return result;
    }
    uint8_t num_points_ascii[XCOMM_MELSEC_2_BYTE];
    xcomm_melsec_byte2ascii(num_points, num_points_ascii);

    result.data[0]  = enq;
    result.data[1]  = stn_no[0];
    result.data[2]  = stn_no[1];
    result.data[3]  = plc_no[0];
    result.data[4]  = plc_no[1];
    result.data[5]  = (op_code == XCOMM_MELSEC_B_OP) ? br_instruction[0] : wr_instruction[0];
    result.data[6]  = (op_code == XCOMM_MELSEC_B_OP) ? br_instruction[1] : wr_instruction[1];
    result.data[7]  = timewait;
    result.data[8]  = addr[0];
    result.data[9]  = addr[1];
    result.data[10] = addr[2];
    result.data[11] = addr[3];
    result.data[12] = addr[4];
    result.data[13] = num_points_ascii[0];
    result.data[14] = num_points_ascii[1];
    result.data[15] = 0;
    result.data[16] = 0;

    uint8_t checksum = 0;
    for (int i = 1; i < result.size - XCOMM_MELSEC_2_BYTE; i++) {
        checksum += result.data[i];
    }
    uint8_t checksum_ascii[XCOMM_MELSEC_2_BYTE];
    xcomm_melsec_byte2ascii(checksum, checksum_ascii);

    result.data[15] = checksum_ascii[0];
    result.data[16] = checksum_ascii[1];
    return result;
}

static inline xcomm_melsec_bytes_t _rd_response_retrieve(
    xcomm_serial_t*        serial,
    xcomm_melsec_operate_t op_code,
    uint8_t                num_points) {
    xcomm_melsec_bytes_t result = {0};
    
    uint8_t fst;
    xcomm_serial.recv(serial, &fst, 1);
    if (fst == stx) {
        /**
         * | 1  |    2    |  2  |      2     |
         * |NAK | STATION | PLC | ERROR_CODE |
         */
        result.size = XCOMM_MELSEC_1_BYTE + XCOMM_MELSEC_2_BYTE +
                      XCOMM_MELSEC_2_BYTE + XCOMM_MELSEC_2_BYTE;
    } else {
        /**
         * | 1  |    2    |  2  | X |  1  |     2    |
         * |STX | STATION | PLC | B | EXT | CHECKSUM |
         */
        result.size = XCOMM_MELSEC_1_BYTE + XCOMM_MELSEC_2_BYTE +
                      XCOMM_MELSEC_2_BYTE +
                      ((op_code == XCOMM_MELSEC_B_OP)
                           ? num_points * XCOMM_MELSEC_CHARS_PER_B_POINT
                           : num_points * XCOMM_MELSEC_CHARS_PER_W_POINT) +
                      XCOMM_MELSEC_1_BYTE + XCOMM_MELSEC_2_BYTE;
    }
    result.data = malloc(result.size);
    if (!result.data) {
        result.size = 0;
        return result;
    }
    result.data[0] = fst;

    if (xcomm_serial.recv(
            serial, (result.data + sizeof(fst)), result.size - sizeof(fst)) !=
        result.size - sizeof(fst)) {
        free(result.data);
        result.size = 0;
        return result;
    }
    return result;
}

xcomm_melsec_device_t* xcomm_melsec_1c_dial(
    xcomm_serial_config_t* config,
    const char* restrict stn_no,
    const char* restrict plc_no) {
    xcomm_logi("%s enter.\n", __FUNCTION__);

    xcomm_melsec_device_t* device = malloc(sizeof(xcomm_melsec_device_t));
    if (!device) {
        return NULL;
    }
    device->opaque = malloc(sizeof(melsec_1c_device_ctx_t));
    if (!device->opaque) {
        free(device);
        return NULL;
    }
    melsec_1c_device_ctx_t* ctx = device->opaque;
    ctx->serial = xcomm_serial.dial(config);
    if (!ctx->serial) {
        free(device->opaque);
        free(device);
        return NULL;
    }
    if (!stn_no || !plc_no) {
        memcpy(ctx->stn_no, &default_stn_no, XCOMM_MELSEC_2_BYTE);
        memcpy(ctx->plc_no, &default_plc_no, XCOMM_MELSEC_2_BYTE);
    }
    if (stn_no && plc_no) {
        memcpy(ctx->stn_no, stn_no, XCOMM_MELSEC_2_BYTE);
        memcpy(ctx->plc_no, plc_no, XCOMM_MELSEC_2_BYTE);
    }
    xcomm_logi("%s leave.\n", __FUNCTION__);
    return device;
}

void xcomm_melsec_1c_close(xcomm_melsec_device_t* device) {
    xcomm_logi("%s enter.\n", __FUNCTION__);

    melsec_1c_device_ctx_t* ctx = device->opaque;
    xcomm_serial.close(ctx->serial);

    free(device->opaque);
    free(device);

    xcomm_logi("%s leave.\n", __FUNCTION__);
}

bool xcomm_melsec_1c_load_bool(
    xcomm_melsec_device_t* device, const char* restrict addr) {
    melsec_1c_device_ctx_t* ctx = device->opaque;

    xcomm_melsec_bytes_t req = _rd_request_marshalling(
        ctx->stn_no, ctx->plc_no, addr, XCOMM_MELSEC_B_OP, 1);

    char reqstr[XCOMM_MELSEC_MAX_MESSAGE_STR_SIZE] = {0};
    xcomm_melsec_bytes2string(req, reqstr, sizeof(reqstr));
    xcomm_logi("[%s] REQUEST FRAME: %s\n", __FUNCTION__, reqstr);

    xcomm_serial.send(ctx->serial, req.data, req.size);
    free(req.data);

    xcomm_melsec_bytes_t rsp =
        _rd_response_retrieve(ctx->serial, XCOMM_MELSEC_B_OP, 1);

    char rspstr[XCOMM_MELSEC_MAX_MESSAGE_STR_SIZE] = {0};
    xcomm_melsec_bytes2string(rsp, rspstr, sizeof(rspstr));
    xcomm_logi("[%s] RESPONSE FRAME: %s\n", __FUNCTION__, rspstr);

    _rd_response_unmarshalling(rsp, XCOMM_MELSEC_B_OP, 1);
    free(rsp.data);
    return false;
}

int8_t xcomm_melsec_1c_load_int8(
    xcomm_melsec_device_t* device, const char* restrict addr) {
}

int16_t xcomm_melsec_1c_load_int16(
    xcomm_melsec_device_t* device, const char* restrict addr) {
}

int32_t xcomm_melsec_1c_load_int32(
    xcomm_melsec_device_t* device, const char* restrict addr) {
}

int64_t xcomm_melsec_1c_load_int64(
    xcomm_melsec_device_t* device, const char* restrict addr) {
}

uint8_t xcomm_melsec_1c_load_uint8(
    xcomm_melsec_device_t* device, const char* restrict addr) {
}

uint16_t xcomm_melsec_1c_load_uint16(
    xcomm_melsec_device_t* device, const char* restrict addr) {
}

uint32_t xcomm_melsec_1c_load_uint32(
    xcomm_melsec_device_t* device, const char* restrict addr) {
}

uint64_t xcomm_melsec_1c_load_uint64(
    xcomm_melsec_device_t* device, const char* restrict addr) {
}

char* xcomm_melsec_1c_load_string(
    xcomm_melsec_device_t* device, const char* restrict addr) {
}

void xcomm_melsec_1c_store_bool(
    xcomm_melsec_device_t* device, const char* restrict addr, bool val) {
}

void xcomm_melsec_1c_store_int8(
    xcomm_melsec_device_t* device, const char* restrict addr, int8_t val) {
}

void xcomm_melsec_1c_store_uint8(
    xcomm_melsec_device_t* device, const char* restrict addr, uint8_t val) {
}

void xcomm_melsec_1c_store_int16(
    xcomm_melsec_device_t* device, const char* restrict addr, int16_t val) {
}

void xcomm_melsec_1c_store_uint16(
    xcomm_melsec_device_t* device, const char* restrict addr, uint16_t val) {
}

void xcomm_melsec_1c_store_int32(
    xcomm_melsec_device_t* device, const char* restrict addr, int32_t val) {
}

void xcomm_melsec_1c_store_uint32(
    xcomm_melsec_device_t* device, const char* restrict addr, uint32_t val) {
}

void xcomm_melsec_1c_store_int64(
    xcomm_melsec_device_t* device, const char* restrict addr, int64_t val) {
}

void xcomm_melsec_1c_store_uint64(
    xcomm_melsec_device_t* device, const char* restrict addr, uint64_t val) {
}

void xcomm_melsec_1c_store_string(
    xcomm_melsec_device_t* device, const char* restrict addr, char* val) {
}

