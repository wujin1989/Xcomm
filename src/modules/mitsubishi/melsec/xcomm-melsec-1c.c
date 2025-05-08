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

typedef struct melsec_1c_fmt01_req_s            melsec_1c_fmt01_req_t;
typedef union melsec_1c_fmt01_rsp_u             melsec_1c_fmt01_rsp_t;

typedef union melsec_1c_fmt01_success_rsp_u     melsec_1c_fmt01_success_rsp_t;
typedef struct melsec_1c_fmt01_failure_rsp_s    melsec_1c_fmt01_failure_rsp_t;

typedef struct melsec_1c_fmt01_rd_success_rsp_s melsec_1c_fmt01_rd_success_rsp_t;
typedef struct melsec_1c_fmt01_wr_success_rsp_s melsec_1c_fmt01_wr_success_rsp_t;

struct melsec_1c_fmt01_req_s {
    char enq;
    char station_no[FRAME_1C_STATION_NO_LENGTH];
    char plc_no[FRAME_1C_PLC_NO_LENGTH];
    char instruction[FRAME_1C_INSTRUCTION_LENGTH];
    char timeout;
    char checksum[FRAME_1C_CHECKSUM_LENGTH];
    char segment[];
};

struct melsec_1c_fmt01_failure_rsp_s {
    char nak;
    char station_no[FRAME_1C_STATION_NO_LENGTH];
    char plc_no[FRAME_1C_PLC_NO_LENGTH];
    char errcode[FRAME_1C_ERRCODE_LENGTH];
};

struct melsec_1c_fmt01_rd_success_rsp_s {
    char stx;
    char station_no[FRAME_1C_STATION_NO_LENGTH];
    char plc_no[FRAME_1C_PLC_NO_LENGTH];
    char etx;
    char checksum[FRAME_1C_CHECKSUM_LENGTH];
    char segment[];
};

struct melsec_1c_fmt01_wr_success_rsp_s {
    char ack;
    char station_no[FRAME_1C_STATION_NO_LENGTH];
    char plc_no[FRAME_1C_PLC_NO_LENGTH];
};

union melsec_1c_fmt01_success_rsp_u {
    melsec_1c_fmt01_rd_success_rsp_t rd;
    melsec_1c_fmt01_wr_success_rsp_t wr;
};

union melsec_1c_fmt01_rsp_u {
    melsec_1c_fmt01_success_rsp_t success;
    melsec_1c_fmt01_failure_rsp_t failure;
};

static int _melsec_1c_segment_length_detect() {

}

bool xcomm_melsec_1c_bool_load(
    xcomm_serial_t* serialptr,
    char*           station_no,
    char*           plc_no,
    const char* restrict addr) {

}

int8_t xcomm_melsec_1c_int8_load(
    xcomm_serial_t* serialptr,
    char*           station_no,
    char*           plc_no,
    const char* restrict addr) {

}

uint8_t xcomm_melsec_1c_uint8_load(
    xcomm_serial_t* serialptr,
    char*           station_no,
    char*           plc_no,
    const char* restrict addr) {

}

int16_t xcomm_melsec_1c_int16_load(
    xcomm_serial_t* serialptr,
    char*           station_no,
    char*           plc_no,
    const char* restrict addr) {

}

uint16_t xcomm_melsec_1c_uint16_load(
    xcomm_serial_t* serialptr,
    char*           station_no,
    char*           plc_no,
    const char* restrict addr) {

}

int32_t xcomm_melsec_1c_int32_load(
    xcomm_serial_t* serialptr,
    char*           station_no,
    char*           plc_no,
    const char* restrict addr) {

}

uint32_t xcomm_melsec_1c_uint32_load(
    xcomm_serial_t* serialptr,
    char*           station_no,
    char*           plc_no,
    const char* restrict addr) {

}

int64_t xcomm_melsec_1c_int64_load(
    xcomm_serial_t* serialptr,
    char*           station_no,
    char*           plc_no,
    const char* restrict addr) {

}

uint64_t xcomm_melsec_1c_uint64_load(
    xcomm_serial_t* serialptr,
    char*           station_no,
    char*           plc_no,
    const char* restrict addr) {

}

const char* xcomm_melsec_1c_string_load(
    xcomm_serial_t* serialptr,
    char*           station_no,
    char*           plc_no,
    const char* restrict addr) {

}

void xcomm_melsec_1c_bool_store(
    xcomm_serial_t* serialptr,
    char*           station_no,
    char*           plc_no,
    const char* restrict addr,
    bool val) {

}

void xcomm_melsec_1c_int8_store(
    xcomm_serial_t* serialptr,
    char*           station_no,
    char*           plc_no,
    const char* restrict addr,
    int8_t val) {

}

void xcomm_melsec_1c_uint8_store(
    xcomm_serial_t* serialptr,
    char*           station_no,
    char*           plc_no,
    const char* restrict addr,
    uint8_t val) {

}

void xcomm_melsec_1c_int16_store(
    xcomm_serial_t* serialptr,
    char*           station_no,
    char*           plc_no,
    const char* restrict addr,
    int16_t val) {

}

void xcomm_melsec_1c_uint16_store(
    xcomm_serial_t* serialptr,
    char*           station_no,
    char*           plc_no,
    const char* restrict addr,
    uint16_t val) {

}

void xcomm_melsec_1c_int32_store(
    xcomm_serial_t* serialptr,
    char*           station_no,
    char*           plc_no,
    const char* restrict addr,
    int32_t val) {

}

void xcomm_melsec_1c_uint32_store(
    xcomm_serial_t* serialptr,
    char*           station_no,
    char*           plc_no,
    const char* restrict addr,
    uint32_t val) {

}

void xcomm_melsec_1c_int64_store(
    xcomm_serial_t* serialptr,
    char*           station_no,
    char*           plc_no,
    const char* restrict addr,
    int64_t val) {

}

void xcomm_melsec_1c_uint64_store(
    xcomm_serial_t* serialptr,
    char*           station_no,
    char*           plc_no,
    const char* restrict addr,
    uint64_t val) {

}

void xcomm_melsec_1c_string_store(
    xcomm_serial_t* serialptr,
    char*           station_no,
    char*           plc_no,
    const char* restrict addr,
    const char* val) {

}