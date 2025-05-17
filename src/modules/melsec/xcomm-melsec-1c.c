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

#include "xcomm-logger.h"
#include "xcomm-melsec-1c.h"
#include "xcomm-melsec-common.h"

typedef struct melsec_1c_device_ctx_s            melsec_1c_device_ctx_t;
typedef struct melsec_1c_read_request_s          melsec_1c_read_request_t;
typedef struct melsec_1c_read_response_s         melsec_1c_read_response_t;
typedef struct melsec_1c_success_read_response_s melsec_1c_success_read_response_t;
typedef struct melsec_1c_failure_read_response_s melsec_1c_failure_read_response_t;
typedef struct melsec_1c_data_a_s                melsec_1c_data_a_t;
typedef struct melsec_1c_data_b_s                melsec_1c_data_b_t;

struct melsec_1c_device_ctx_s {
    char            stn_no[XCOMM_MELSEC_2_BYTE];
    char            plc_no[XCOMM_MELSEC_2_BYTE];
    xcomm_serial_t* serial;
};

struct melsec_1c_data_a_s {
    uint8_t addr[XCOMM_MELSEC_5_BYTE];
    uint8_t points[XCOMM_MELSEC_2_BYTE];
};

struct melsec_1c_read_request_s {
    uint8_t            enq;
    uint8_t            stn_no[XCOMM_MELSEC_2_BYTE];
    uint8_t            plc_no[XCOMM_MELSEC_2_BYTE];
    uint8_t            instruction[XCOMM_MELSEC_2_BYTE];
    uint8_t            timewait;
    melsec_1c_data_a_t data_a;
    uint8_t            checksum[XCOMM_MELSEC_2_BYTE];
};

struct melsec_1c_data_b_s {
    xcomm_melsec_byte_sequence_t buf;
};

struct melsec_1c_success_read_response_s {
    uint8_t            stx;
    uint8_t            stn_no[XCOMM_MELSEC_2_BYTE];
    uint8_t            plc_no[XCOMM_MELSEC_2_BYTE];
    melsec_1c_data_b_t data_b;
    uint8_t            etx;
    uint8_t            checksum[XCOMM_MELSEC_2_BYTE];
};

struct melsec_1c_failure_read_response_s {
    uint8_t nak;
    uint8_t stn_no[XCOMM_MELSEC_2_BYTE];
    uint8_t plc_no[XCOMM_MELSEC_2_BYTE];
    uint8_t errcode[XCOMM_MELSEC_2_BYTE];
};

struct melsec_1c_read_response_s {
    union {
        melsec_1c_success_read_response_t success;
        melsec_1c_failure_read_response_t failure;
    };
    bool status;
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

static inline int _melsec_1c_read_request_marshalling(
    melsec_1c_read_request_t* req, xcomm_melsec_byte_sequence_t* stm) {
    size_t size = XCOMM_MELSEC_1_BYTE +         // ENQ
                  XCOMM_MELSEC_2_BYTE +         // STATION
                  XCOMM_MELSEC_2_BYTE +         // PLC
                  XCOMM_MELSEC_2_BYTE +         // INSTRUCTION
                  XCOMM_MELSEC_1_BYTE +         // TIMEWAIT
                  XCOMM_MELSEC_5_BYTE +         // START_ADDRESS
                  XCOMM_MELSEC_2_BYTE +         // POINTS
                  XCOMM_MELSEC_2_BYTE;          // CHECKSUM

    uint8_t* data = malloc(size);
    if (!data) {
        stm->data = NULL;
        stm->size = 0;
        return -1;
    }
    size_t offset = 0;
    data[offset++] = req->enq;
    data[offset++] = req->stn_no[0];
    data[offset++] = req->stn_no[1];
    data[offset++] = req->plc_no[0];
    data[offset++] = req->plc_no[1];
    data[offset++] = req->instruction[0];
    data[offset++] = req->instruction[1];
    data[offset++] = req->timewait;
    data[offset++] = req->data_a.addr[0];
    data[offset++] = req->data_a.addr[1];
    data[offset++] = req->data_a.addr[2];
    data[offset++] = req->data_a.addr[3];
    data[offset++] = req->data_a.addr[4];
    data[offset++] = req->data_a.points[0];
    data[offset++] = req->data_a.points[1];
    data[offset++] = req->checksum[0];
    data[offset++] = req->checksum[1];

    stm->data = data;
    stm->size = size;
    return 0;
}

static inline void
_melsec_1c_send_request(xcomm_serial_t* serial, uint8_t* data, size_t size) {
    char reqframe[XCOMM_MELSEC_512_BYTE] = {0};
    for (size_t i = 0; i < size; i++) {
        sprintf(reqframe + i * 3, "%02X ", data[i]);
    }
    xcomm_logi("REQUEST FRAME(Hex): %s\n", reqframe);

    int ret = xcomm_serial.send(serial, data, size);
    if (ret != size) {
        xcomm_loge("send error. ret=%d, expected=%zu\n", ret, size);
        return;
    }
    return;
}

static void _melsec_1c_recv_response(
    xcomm_serial_t*               serial,
    uint64_t                      size,
    xcomm_melsec_byte_sequence_t* rsp_stm) {
    uint8_t fst;
    xcomm_serial.recv(serial, &fst, 1);
    if (fst == nak) {
        rsp_stm->size = 
            XCOMM_MELSEC_1_BYTE + 
            XCOMM_MELSEC_2_BYTE +
            XCOMM_MELSEC_2_BYTE + 
            XCOMM_MELSEC_2_BYTE;
    } else {
        rsp_stm->size = 
            XCOMM_MELSEC_1_BYTE + 
            XCOMM_MELSEC_2_BYTE +
            XCOMM_MELSEC_2_BYTE + 
            size + 
            XCOMM_MELSEC_1_BYTE +
            XCOMM_MELSEC_2_BYTE;
    }
    rsp_stm->data = malloc(rsp_stm->size);
    if (!rsp_stm->data) {
        return;
    }
    rsp_stm->data[0] = fst;

    if (xcomm_serial.recv(
            serial,
            (rsp_stm->data + sizeof(fst)),
            rsp_stm->size - sizeof(fst)) != rsp_stm->size - sizeof(fst)) {
        return;
    }
    char rspframe[XCOMM_MELSEC_512_BYTE] = {0};
    for (size_t i = 0; i < rsp_stm->size; i++) {
        sprintf(rspframe + i * 3, "%02X ", rsp_stm->data[i]);
    }
    xcomm_logi("RESPONSE FRAME(Hex): %s\n", rspframe);
    return;
}

static int _melsec_1c_read_response_unmarshalling(
    xcomm_melsec_byte_sequence_t* rsp_stm, melsec_1c_read_response_t* rsp) {
    if (!rsp_stm || !rsp_stm->data || rsp_stm->size == 0) {
        return -1;
    }
    if (rsp_stm->data[0] == stx) {
        size_t offset = 0;
        rsp->success.stx = rsp_stm->data[offset++];
        rsp->success.stn_no[0] = rsp_stm->data[offset++];
        rsp->success.stn_no[1] = rsp_stm->data[offset++];
        rsp->success.plc_no[0] = rsp_stm->data[offset++];
        rsp->success.plc_no[1] = rsp_stm->data[offset++];

        size_t data_b_len = 
            rsp_stm->size - 
            offset - 
            XCOMM_MELSEC_1_BYTE -  //ETX
            XCOMM_MELSEC_2_BYTE;   //CHECKSUM

        rsp->success.data_b.buf.size = data_b_len;
        rsp->success.data_b.buf.data = malloc(data_b_len);
        if (!rsp->success.data_b.buf.data) {
            return -1;
        }
        memcpy(
            rsp->success.data_b.buf.data, rsp_stm->data + offset, data_b_len);
        offset += data_b_len;

        rsp->success.etx = rsp_stm->data[offset++];
        rsp->success.checksum[0] = rsp_stm->data[offset++];
        rsp->success.checksum[1] = rsp_stm->data[offset++];
        rsp->status = true;
    } else if (rsp_stm->data[0] == nak) {
        size_t offset = 0;
        rsp->failure.nak = rsp_stm->data[offset++];
        rsp->failure.stn_no[0]  = rsp_stm->data[offset++];
        rsp->failure.stn_no[1]  = rsp_stm->data[offset++];
        rsp->failure.plc_no[0]  = rsp_stm->data[offset++];
        rsp->failure.plc_no[1]  = rsp_stm->data[offset++];
        rsp->failure.errcode[0] = rsp_stm->data[offset++];
        rsp->failure.errcode[1] = rsp_stm->data[offset++];
        rsp->status = false;
    } else {
        xcomm_loge("unknown response error.\n");
        return -1;
    }
    return 0;
}

static inline void _melsec_1c_read_request_build(
    uint8_t                   stn_no[XCOMM_MELSEC_2_BYTE],
    uint8_t                   plc_no[XCOMM_MELSEC_2_BYTE],
    xcomm_melsec_operate_t    op_code,
    const char* restrict      addr,
    uint8_t                   points,
    melsec_1c_read_request_t* req) {
    size_t  offset = 0;
    uint8_t points_ascii[XCOMM_MELSEC_2_BYTE];
    uint8_t checksum_ascii[XCOMM_MELSEC_2_BYTE];
    uint8_t checksum_buf
        [XCOMM_MELSEC_2_BYTE + XCOMM_MELSEC_2_BYTE + XCOMM_MELSEC_2_BYTE +
         XCOMM_MELSEC_1_BYTE + XCOMM_MELSEC_5_BYTE + XCOMM_MELSEC_2_BYTE];

    xcomm_melsec_byte_to_ascii(points, points_ascii);

    req->enq = enq;
    req->stn_no[0] = stn_no[0];
    req->stn_no[1] = stn_no[1];
    req->plc_no[0] = plc_no[0];
    req->plc_no[1] = plc_no[1];
    req->instruction[0] = (op_code == XCOMM_MELSEC_B_OP) ? br_instruction[0] : wr_instruction[0];
    req->instruction[1] = (op_code == XCOMM_MELSEC_B_OP) ? br_instruction[1] : wr_instruction[1];
    req->timewait = default_timewait;

    memcpy(req->data_a.addr, addr, strlen(addr));

    req->data_a.points[0] = points_ascii[0];
    req->data_a.points[1] = points_ascii[1];

    checksum_buf[offset++] = req->stn_no[0];
    checksum_buf[offset++] = req->stn_no[1];
    checksum_buf[offset++] = req->plc_no[0];
    checksum_buf[offset++] = req->plc_no[1];
    checksum_buf[offset++] = req->instruction[0];
    checksum_buf[offset++] = req->instruction[1];
    checksum_buf[offset++] = req->timewait;

    for (int i = 0; i < XCOMM_MELSEC_5_BYTE; ++i) {
        checksum_buf[offset++] = req->data_a.addr[i];
    }
    checksum_buf[offset++] = req->data_a.points[0];
    checksum_buf[offset++] = req->data_a.points[1];

    uint8_t sum = 0;
    for (size_t i = 0; i < sizeof(checksum_buf); i++) {
        sum += checksum_buf[i];
    }
    xcomm_melsec_byte_to_ascii(sum, checksum_ascii);

    req->checksum[0] = checksum_ascii[0];
    req->checksum[1] = checksum_ascii[1];
}

static void _melsec_1c_read_response_parse(
    bool                           isstr,
    xcomm_melsec_operate_t         op_code,
    melsec_1c_read_response_t*     rsp,
    xcomm_melsec_flexible_value_t* val) {
    if (isstr) {
        if (val->str = calloc(rsp->success.data_b.buf.size, sizeof(char))) {
            memcpy(
                val->str,
                rsp->success.data_b.buf.data,
                rsp->success.data_b.buf.size);
        }
    } else {
        char data_b_str[XCOMM_MELSEC_512_BYTE] = {0};
        memcpy(
            data_b_str,
            rsp->success.data_b.buf.data,
            rsp->success.data_b.buf.size);

        if (op_code == XCOMM_MELSEC_B_OP) {
            val->u64 = strtoll(data_b_str, NULL, 2);
        }
        if (op_code == XCOMM_MELSEC_W_OP) {
            val->u64 = strtoll(data_b_str, NULL, 16);
        }
    }
    return;
}

static int _melsec_1c_load_value(
    xcomm_melsec_device_t*         device,
    const char* restrict           addr,
    xcomm_melsec_operate_t         op_code,
    size_t                         len,
    bool                           isstr,
    xcomm_melsec_flexible_value_t* val) {
    melsec_1c_device_ctx_t* ctx = device->opaque;

    size_t points = 0;
    if (op_code == XCOMM_MELSEC_B_OP) {
        points = 
            (len / XCOMM_MELSEC_CHARS_PER_B_POINT);
    }
    if (op_code == XCOMM_MELSEC_W_OP) {
        points = 
            (len / XCOMM_MELSEC_CHARS_PER_W_POINT);
    }

    melsec_1c_read_request_t req = {0};
    _melsec_1c_read_request_build(
        ctx->stn_no, ctx->plc_no, op_code, addr, points, &req);

    xcomm_melsec_byte_sequence_t req_stm = {0};
    if (_melsec_1c_read_request_marshalling(&req, &req_stm) < 0) {
        free(req_stm.data);
        return -1;
    }
    _melsec_1c_send_request(ctx->serial, req_stm.data, req_stm.size);

    xcomm_melsec_byte_sequence_t rsp_stm = {0};
    _melsec_1c_recv_response(ctx->serial, len, &rsp_stm);

    melsec_1c_read_response_t rsp;
    if (_melsec_1c_read_response_unmarshalling(&rsp_stm, &rsp) <
        0) {
        free(req_stm.data);
        free(rsp_stm.data);
        return -1;
    }
    if (rsp.status) {
        _melsec_1c_read_response_parse(isstr, op_code, &rsp, val);
    } else {
        xcomm_logw(
            "received failure response: errcode=%02X%02X\n",
            rsp.failure.errcode[0],
            rsp.failure.errcode[1]);
    }
    if (rsp.status) {
        free(rsp.success.data_b.buf.data);
    }
    free(req_stm.data);
    free(rsp_stm.data);
    return 0;
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

    size_t points = 1;
    size_t length = points * XCOMM_MELSEC_CHARS_PER_B_POINT;

    int ret = _melsec_1c_load_value(
        device, addr, XCOMM_MELSEC_B_OP, length, false, &val);
    if (!ret) {
        *dst = val.b;
    }
    return ret;
}

int xcomm_melsec_1c_load_int16(
    xcomm_melsec_device_t* device, const char* restrict addr, int16_t* dst) {
    xcomm_melsec_flexible_value_t val = {0};

    size_t points = 1;
    size_t length = points * XCOMM_MELSEC_CHARS_PER_W_POINT;

    int ret = _melsec_1c_load_value(
        device, addr, XCOMM_MELSEC_W_OP, length, false, &val);
    if (!ret) {
        *dst = val.i16;
    }
    return ret;
}

int xcomm_melsec_1c_load_uint16(
    xcomm_melsec_device_t* device, const char* restrict addr, uint16_t* dst) {
    xcomm_melsec_flexible_value_t val = {0};

    size_t points = 1;
    size_t length = points * XCOMM_MELSEC_CHARS_PER_W_POINT;

    int ret = _melsec_1c_load_value(
        device, addr, XCOMM_MELSEC_W_OP, length, false, &val);
    if (!ret) {
        *dst = val.u16;
    }
    return ret;
}

int xcomm_melsec_1c_load_int32(
    xcomm_melsec_device_t* device, const char* restrict addr, int32_t* dst) {
    xcomm_melsec_flexible_value_t val = {0};

    size_t points = 2;
    size_t length = points * XCOMM_MELSEC_CHARS_PER_W_POINT;

    int ret = _melsec_1c_load_value(
        device, addr, XCOMM_MELSEC_W_OP, length, false, &val);
    if (!ret) {
        *dst = val.i32;
    }
    return ret;
}

int xcomm_melsec_1c_load_uint32(
    xcomm_melsec_device_t* device, const char* restrict addr, uint32_t* dst) {
    xcomm_melsec_flexible_value_t val = {0};

    size_t points = 2;
    size_t length = points * XCOMM_MELSEC_CHARS_PER_W_POINT;

    int ret = _melsec_1c_load_value(
        device, addr, XCOMM_MELSEC_W_OP, length, false, &val);
    if (!ret) {
        *dst = val.u32;
    }
    return ret;
}

int xcomm_melsec_1c_load_int64(
    xcomm_melsec_device_t* device, const char* restrict addr, int64_t* dst) {
    xcomm_melsec_flexible_value_t val = {0};

    size_t points = 4;
    size_t length = points * XCOMM_MELSEC_CHARS_PER_W_POINT;

    int ret = _melsec_1c_load_value(
        device, addr, XCOMM_MELSEC_W_OP, length, false, &val);
    if (!ret) {
        *dst = val.i64;
    }
    return ret;
}

int xcomm_melsec_1c_load_uint64(
    xcomm_melsec_device_t* device, const char* restrict addr, uint64_t* dst) {
    xcomm_melsec_flexible_value_t val = {0};

    size_t points = 4;
    size_t length = points * XCOMM_MELSEC_CHARS_PER_W_POINT;

    int ret = _melsec_1c_load_value(
        device, addr, XCOMM_MELSEC_W_OP, length, false, &val);
    if (!ret) {
        *dst = val.u64;
    }
    return ret;
}

int xcomm_melsec_1c_load_float(
    xcomm_melsec_device_t* device, const char* restrict addr, float* dst) {
    xcomm_melsec_flexible_value_t val = {0};

    size_t points = 2;
    size_t length = points * XCOMM_MELSEC_CHARS_PER_W_POINT;

    int ret = _melsec_1c_load_value(
        device, addr, XCOMM_MELSEC_W_OP, length, false, &val);
    if (!ret) {
        *dst = val.f32;
    }
    return ret;
}

int xcomm_melsec_1c_load_double(
    xcomm_melsec_device_t* device, const char* restrict addr, double* dst) {
    xcomm_melsec_flexible_value_t val = {0};

    size_t points = 4;
    size_t length = points * XCOMM_MELSEC_CHARS_PER_W_POINT;

    int ret = _melsec_1c_load_value(
        device, addr, XCOMM_MELSEC_W_OP, length, false, &val);
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
        device, addr, XCOMM_MELSEC_W_OP, dstlen, true, &val);
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
