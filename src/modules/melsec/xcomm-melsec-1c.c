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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "xcomm-melsec-1c.h"
#include "xcomm-melsec-common.h"

typedef struct melsec_1c_device_ctx_s  melsec_1c_device_ctx_t;

struct melsec_1c_device_ctx_s {
    char stn_no[XCOMM_MELSEC_2_BYTE];
    char plc_no[XCOMM_MELSEC_2_BYTE];
    xcomm_serial_t* serial;
};

static const uint8_t default_stn_no[] = {0x30, 0x30};
static const uint8_t default_plc_no[] = {0x46, 0x46};
static const uint8_t default_timewait = 0x46;

static const uint8_t enq = 0x05;
static const uint8_t stx = 0x02;
static const uint8_t etx = 0x03;
static const uint8_t nak = 0x15;

static const uint8_t br_instruction[] = {0x45, 0x52};
static const uint8_t wr_instruction[] = {0x57, 0x52};
static const uint8_t bw_instruction[] = {0x42, 0x57};
static const uint8_t ww_instruction[] = {0x57, 0x57};

static inline xcomm_melsec_bytes_t 
_rd_request_marshalling(
    uint8_t*               stn_no,
    uint8_t*               plc_no,
    const char* restrict   addr,
    xcomm_melsec_operate_t op_code,
    uint8_t                npoints) {
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
    uint8_t npoints_ascii[XCOMM_MELSEC_2_BYTE];
    xcomm_melsec_byte_to_ascii(npoints, npoints_ascii);

    result.data[0]  = enq;
    result.data[1]  = stn_no[0];
    result.data[2]  = stn_no[1];
    result.data[3]  = plc_no[0];
    result.data[4]  = plc_no[1];
    result.data[5]  = (op_code == XCOMM_MELSEC_B_OP) ? br_instruction[0] : wr_instruction[0];
    result.data[6]  = (op_code == XCOMM_MELSEC_B_OP) ? br_instruction[1] : wr_instruction[1];
    result.data[7]  = default_timewait;
    result.data[8]  = addr[0];
    result.data[9]  = addr[1];
    result.data[10] = addr[2];
    result.data[11] = addr[3];
    result.data[12] = addr[4];
    result.data[13] = npoints_ascii[0];
    result.data[14] = npoints_ascii[1];
    result.data[15] = 0;
    result.data[16] = 0;

    uint8_t checksum[XCOMM_MELSEC_2_BYTE];
    xcomm_melsec_ascii_checksum_calc((result.data + 1), result.size - 3, checksum);

    result.data[15] = checksum[0];
    result.data[16] = checksum[1];
    return result;
}

static inline void
_rd_request_send(xcomm_serial_t* serial, uint8_t* data, size_t size) {
    char reqframe[XCOMM_MELSEC_MAX_MESSAGE_STR_SIZE] = {0};

    xcomm_melsec_bytes_to_hex_string(
        data, size, reqframe, sizeof(reqframe), " ");
    xcomm_logi(
        "TX REQUEST FRAME(Hex String): %s\n", reqframe);

    int ret = xcomm_serial.send(serial, data, size);
    if (ret != size) {
        xcomm_loge("Send error. ret=%d, expected=%zu\n", ret, size);
        return;
    }
    return;
}

static inline xcomm_melsec_bytes_t _rd_response_recv(
    xcomm_serial_t*        serial,
    xcomm_melsec_operate_t op_code,
    uint8_t                npoints,
    bool                   is_string,
    uint64_t               str_length) {
    xcomm_melsec_bytes_t result = {0};
    
    uint8_t fst;
    xcomm_serial.recv(serial, &fst, 1);
    if (fst == nak) {
        /**
         * | 1  |    2    |  2  |      2     |
         * |NAK | STATION | PLC | ERROR_CODE |
         */
        result.size = XCOMM_MELSEC_1_BYTE + XCOMM_MELSEC_2_BYTE +
                      XCOMM_MELSEC_2_BYTE + XCOMM_MELSEC_2_BYTE;
    } else {
        /**
         * | 1  |    2    |  2  | X |  1  |     2    |
         * |STX | STATION | PLC | B | ETX | CHECKSUM |
         */
        if (is_string) {
            result.size = XCOMM_MELSEC_1_BYTE + XCOMM_MELSEC_2_BYTE +
                          XCOMM_MELSEC_2_BYTE + str_length +
                          XCOMM_MELSEC_1_BYTE + XCOMM_MELSEC_2_BYTE;
        } else {
            result.size = XCOMM_MELSEC_1_BYTE + XCOMM_MELSEC_2_BYTE +
                          XCOMM_MELSEC_2_BYTE +
                          ((op_code == XCOMM_MELSEC_B_OP)
                               ? npoints * XCOMM_MELSEC_CHARS_PER_B_POINT
                               : npoints * XCOMM_MELSEC_CHARS_PER_W_POINT) +
                          XCOMM_MELSEC_1_BYTE + XCOMM_MELSEC_2_BYTE;
        }
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
    char rspframe[XCOMM_MELSEC_MAX_MESSAGE_STR_SIZE] = {0};
    xcomm_melsec_bytes_to_hex_string(
        result.data, result.size, rspframe, sizeof(rspframe), " ");
    xcomm_logi("RX RESPONSE FRAME(Hex String): %s\n", rspframe);

    return result;
}

static inline xcomm_melsec_flexible_value_t 
_normal_rd_response_unmarshalling(
    xcomm_melsec_bytes_t   rsp,
    xcomm_melsec_operate_t op_code,
    uint8_t                npoints,
    bool                   is_string,
    uint64_t               str_length) {
    size_t  data_offset = 0;
    size_t  data_length = 0;
    int64_t parsed_value = 0;

    xcomm_melsec_flexible_value_t result           = {0};
    uint8_t calc_checksum[XCOMM_MELSEC_2_BYTE]     = {0};
    char ascii_buffer[XCOMM_MELSEC_MAX_MESSAGE_STR_SIZE] = {0};

    data_offset += XCOMM_MELSEC_1_BYTE 
                + XCOMM_MELSEC_2_BYTE 
                + XCOMM_MELSEC_2_BYTE;

    if (is_string) {
        data_length = str_length;
    } else {
        data_length = (op_code == XCOMM_MELSEC_B_OP)
                    ? npoints * XCOMM_MELSEC_CHARS_PER_B_POINT
                    : npoints * XCOMM_MELSEC_CHARS_PER_W_POINT;
    }
    

    if (rsp.size < data_offset + data_length + 
        XCOMM_MELSEC_1_BYTE + 
        XCOMM_MELSEC_2_BYTE) {
        xcomm_loge(
            "Invalid response size. bytes.size=%zu, expected=%zu\n",
            rsp.size,
            data_offset + data_length + 
            XCOMM_MELSEC_1_BYTE + 
            XCOMM_MELSEC_2_BYTE);
        return result;
    }
    xcomm_melsec_bytes_t data_segment = {
        .size = data_length, 
        .data = malloc(data_length)
    };
    if (!data_segment.data) {
        xcomm_loge("No memory.\n");
        return result;
    }
    memcpy(data_segment.data, rsp.data + data_offset, data_segment.size);
    data_offset += data_segment.size + XCOMM_MELSEC_1_BYTE;

    xcomm_melsec_ascii_checksum_calc(
        (rsp.data + 1), 
        rsp.size - XCOMM_MELSEC_3_BYTE, 
        calc_checksum);

    if (calc_checksum[0] != rsp.data[data_offset] ||
        calc_checksum[1] != rsp.data[data_offset + 1]) {
        xcomm_loge(
            "Checksum error. Calculated=%02X%02X, Received=%02X%02X\n",
            calc_checksum[0],
            calc_checksum[1],
            rsp.data[data_offset],
            rsp.data[data_offset + 1]);

        free(data_segment.data);
        return result;
    }
    if (is_string) {
        result.str = malloc(data_segment.size);
        if (!result.str) {
            xcomm_loge("No memory.\n");
            return result;
        }
        memset(result.str, 0, data_segment.size);
        memcpy(result.str, data_segment.data, data_segment.size);
    } else {
        xcomm_melsec_bytes_to_ascii_string(
            data_segment.data,
            data_segment.size,
            ascii_buffer,
            sizeof(ascii_buffer),
            "");
        result.i64 = (op_code == XCOMM_MELSEC_B_OP) 
                   ? strtoll(ascii_buffer, NULL, 2)
                   : strtoll(ascii_buffer, NULL, 16);
    }
    free(data_segment.data);
    return result;
}

static inline void
_abnormal_rd_response_unmarshalling(xcomm_melsec_bytes_t rsp) {
    size_t offset =
        XCOMM_MELSEC_1_BYTE + XCOMM_MELSEC_2_BYTE + XCOMM_MELSEC_2_BYTE;

    xcomm_melsec_bytes_t code = {
        .size = XCOMM_MELSEC_2_BYTE, .data = malloc(XCOMM_MELSEC_2_BYTE)};
    if (!code.data) {
        xcomm_loge("No memory.\n");
        return;
    }
    memcpy(code.data, rsp.data + offset, XCOMM_MELSEC_2_BYTE);

    char codestr[XCOMM_MELSEC_MAX_MESSAGE_STR_SIZE] = {0};
    xcomm_melsec_bytes_to_ascii_string(
        code.data, code.size, codestr, XCOMM_MELSEC_MAX_MESSAGE_STR_SIZE, "");

    xcomm_loge("[%s] Error Code: %s\n", __FUNCTION__, codestr);
}

static int _melsec_1c_load_value(
    xcomm_melsec_device_t*         device,
    const char* restrict           addr,
    xcomm_melsec_operate_t         op_code,
    uint64_t                       npoints,
    xcomm_melsec_flexible_value_t* val,
    bool                           is_string,
    uint64_t                       str_length) {
    xcomm_melsec_bytes_t    req = {0};
    xcomm_melsec_bytes_t    rsp = {0};
    melsec_1c_device_ctx_t* ctx = device->opaque;

    req = _rd_request_marshalling(
        ctx->stn_no, ctx->plc_no, addr, op_code, npoints);
    if (!req.data) {
        goto fail;
    }
    _rd_request_send(ctx->serial, req.data, req.size);
    rsp = _rd_response_recv(ctx->serial, op_code, npoints, is_string, str_length);
    if (!rsp.data) {
        goto fail;
    }
    if (rsp.data[0] == stx) {
        *val = _normal_rd_response_unmarshalling(rsp, op_code, npoints, is_string, str_length);
    } else if (rsp.data[0] == nak) {
        _abnormal_rd_response_unmarshalling(rsp);
        goto fail;
    } else {
        xcomm_loge("Unknown Frame Error.\n");
        goto fail;
    }
    free(req.data); free(rsp.data);
    return 0;
fail:
    if (req.data) { free(req.data); }
    if (rsp.data) { free(rsp.data); }
    return -1;
}

xcomm_melsec_device_t* xcomm_melsec_1c_dial(
    xcomm_serial_config_t* config,
    const char* restrict   stn_no,
    const char* restrict   plc_no) {
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

int xcomm_melsec_1c_load_bool(
    xcomm_melsec_device_t* device, const char* restrict addr, bool* dst) {
    xcomm_melsec_flexible_value_t val = {0};
    int ret =
        _melsec_1c_load_value(device, addr, XCOMM_MELSEC_B_OP, 1, &val, false, 0);
    if (!ret) {
        *dst = val.b;
    }
    return ret;
}

int xcomm_melsec_1c_load_int16(
    xcomm_melsec_device_t* device, const char* restrict addr, int16_t* dst) {
    xcomm_melsec_flexible_value_t val = {0};
    int ret = _melsec_1c_load_value(
        device, addr, XCOMM_MELSEC_W_OP, 1, &val, false, 0);
    if (!ret) {
        *dst = val.i16;
    }
    return ret;
}

int xcomm_melsec_1c_load_uint16(
    xcomm_melsec_device_t* device, const char* restrict addr, uint16_t* dst) {
    xcomm_melsec_flexible_value_t val = {0};
    int ret = _melsec_1c_load_value(
        device, addr, XCOMM_MELSEC_W_OP, 1, &val, false, 0);
    if (!ret) {
        *dst = val.u16;
    }
    return ret;
}

int xcomm_melsec_1c_load_int32(
    xcomm_melsec_device_t* device, const char* restrict addr, int32_t* dst) {
    xcomm_melsec_flexible_value_t val = {0};
    int ret = _melsec_1c_load_value(
        device, addr, XCOMM_MELSEC_W_OP, 2, &val, false, 0);
    if (!ret) {
        *dst = val.i32;
    }
    return ret;
}

int xcomm_melsec_1c_load_uint32(
    xcomm_melsec_device_t* device, const char* restrict addr, uint32_t* dst) {
    xcomm_melsec_flexible_value_t val = {0};
    int ret = _melsec_1c_load_value(
        device, addr, XCOMM_MELSEC_W_OP, 2, &val, false, 0);
    if (!ret) {
        *dst = val.u32;
    }
    return ret;
}

int xcomm_melsec_1c_load_int64(
    xcomm_melsec_device_t* device, const char* restrict addr, int64_t* dst) {
    xcomm_melsec_flexible_value_t val = {0};
    int ret = _melsec_1c_load_value(
        device, addr, XCOMM_MELSEC_W_OP, 4, &val, false, 0);
    if (!ret) {
        *dst = val.i64;
    }
    return ret;
}

int xcomm_melsec_1c_load_uint64(
    xcomm_melsec_device_t* device, const char* restrict addr, uint64_t* dst) {
    xcomm_melsec_flexible_value_t val = {0};
    int ret = _melsec_1c_load_value(
        device, addr, XCOMM_MELSEC_W_OP, 4, &val, false, 0);
    if (!ret) {
        *dst = val.u64;
    }
    return ret;
}

int xcomm_melsec_1c_load_float(
    xcomm_melsec_device_t* device, const char* restrict addr, float* dst) {
    xcomm_melsec_flexible_value_t val = {0};
    int ret = _melsec_1c_load_value(
        device, addr, XCOMM_MELSEC_W_OP, 2, &val, false, 0);
    if (!ret) {
        *dst = val.f32;
    }
    return ret;
}

int xcomm_melsec_1c_load_double(
    xcomm_melsec_device_t* device, const char* restrict addr, double* dst) {
    xcomm_melsec_flexible_value_t val = {0};
    int ret = _melsec_1c_load_value(
        device, addr, XCOMM_MELSEC_W_OP, 4, &val, false, 0);
    if (!ret) {
        *dst = val.f64;
    }
    return ret;
}

int xcomm_melsec_1c_load_string(
    xcomm_melsec_device_t* device,
    const char* restrict   addr,
    char**                 dst,
    uint64_t               dstlen) {
    xcomm_melsec_flexible_value_t val = {0};
    int ret = _melsec_1c_load_value(
        device, addr, XCOMM_MELSEC_W_OP, (uint64_t)ceil(dstlen / 2.0), &val, true, dstlen);
    if (!ret) {
        *dst = val.str;
    }
    return ret;
}

int xcomm_melsec_1c_store_bool(
    xcomm_melsec_device_t* device, const char* restrict addr, bool src) {

}

int xcomm_melsec_1c_store_int16(
    xcomm_melsec_device_t* device, const char* restrict addr, int16_t src) {
}

int xcomm_melsec_1c_store_uint16(
    xcomm_melsec_device_t* device, const char* restrict addr, uint16_t src) {
}

int xcomm_melsec_1c_store_int32(
    xcomm_melsec_device_t* device, const char* restrict addr, int32_t src) {

}

int xcomm_melsec_1c_store_uint32(
    xcomm_melsec_device_t* device, const char* restrict addr, uint32_t src) {
}

int xcomm_melsec_1c_store_int64(
    xcomm_melsec_device_t* device, const char* restrict addr, int64_t src) {
}

int xcomm_melsec_1c_store_uint64(
    xcomm_melsec_device_t* device, const char* restrict addr, uint64_t src) {
}

int xcomm_melsec_1c_store_float(
    xcomm_melsec_device_t* device, const char* restrict addr, float src) {
}

int xcomm_melsec_1c_store_double(
    xcomm_melsec_device_t* device, const char* restrict addr, double src) {
}

int xcomm_melsec_1c_store_string(
    xcomm_melsec_device_t* device,
    const char* restrict   addr,
    char*                  src,
    int                    slen) {

}