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

#include "xcomm-melsec-1c.h"
#include "xcomm-melsec-common.h"

typedef struct melsec_1c_device_ctx_s melsec_1c_device_ctx_t;

struct melsec_1c_device_ctx_s {
    char            stn_no[XCOMM_MELSEC_2_BYTE];
    char            plc_no[XCOMM_MELSEC_2_BYTE];
    xcomm_serial_t* serial;
};

static const uint8_t default_stn_no[] = {0x30, 0x30};
static const uint8_t default_plc_no[] = {0x46, 0x46};

void xcomm_melsec_1c_close(xcomm_melsec_device_t* device) {
    xcomm_logi("%s enter.\n", __FUNCTION__);

    melsec_1c_device_ctx_t* ctx = device->opaque;
    xcomm_serial.close(ctx->serial);

    free(device->opaque);
    free(device);

    xcomm_logi("%s leave.\n", __FUNCTION__);
}

xcomm_melsec_device_t* xcomm_melsec_1c_dial(
    xcomm_serial_config_t* config,
    const char* restrict stn_no,
    const char* restrict plc_no) {
    xcomm_logi("%s enter.\n", __FUNCTION__);

    xcomm_melsec_device_t* device = malloc(sizeof(xcomm_melsec_device_t));
    if (!device) {
        xcomm_loge("no memory.\n");
        return NULL;
    }
    device->opaque = malloc(sizeof(melsec_1c_device_ctx_t));
    if (!device->opaque) {
        xcomm_loge("no memory.\n");
        free(device);
        return NULL;
    }
    melsec_1c_device_ctx_t* ctx = device->opaque;
    ctx->serial = xcomm_serial.dial(config);

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

bool xcomm_melsec_1c_load_bool(
    xcomm_melsec_device_t* device, const char* restrict addr) {

}

int8_t xcomm_melsec_1c_load_int8(
    xcomm_melsec_device_t* device, const char* restrict addr) {

}

uint8_t xcomm_melsec_1c_load_uint8(
    xcomm_melsec_device_t* device, const char* restrict addr) {

}

int16_t xcomm_melsec_1c_load_int16(
    xcomm_melsec_device_t* device, const char* restrict addr) {

}

uint16_t xcomm_melsec_1c_load_uint16(
    xcomm_melsec_device_t* device, const char* restrict addr) {

}

int32_t xcomm_melsec_1c_load_int32(
    xcomm_melsec_device_t* device, const char* restrict addr) {

}

uint32_t xcomm_melsec_1c_load_uint32(
    xcomm_melsec_device_t* device, const char* restrict addr) {

}

int64_t xcomm_melsec_1c_load_int64(
    xcomm_melsec_device_t* device, const char* restrict addr) {

}

uint64_t xcomm_melsec_1c_load_uint64(
    xcomm_melsec_device_t* device, const char* restrict addr) {

}

char* xcomm_melsec_1c_load_string(
    xcomm_melsec_device_t* device, const char* restrict addr) {

}

void xcomm_melsec_1c_store_bool(
    xcomm_melsec_device_t* device, const char* restrict addr,
    bool val) {

}

void xcomm_melsec_1c_store_int8(
    xcomm_melsec_device_t* device, const char* restrict addr,
    int8_t val) {

}

void xcomm_melsec_1c_store_uint8(
    xcomm_melsec_device_t* device, const char* restrict addr,
    uint8_t val) {

}

void xcomm_melsec_1c_store_int16(
    xcomm_melsec_device_t* device, const char* restrict addr,
    int16_t val) {

}

void xcomm_melsec_1c_store_uint16(
    xcomm_melsec_device_t* device, const char* restrict addr,
    uint16_t val) {

}

void xcomm_melsec_1c_store_int32(
    xcomm_melsec_device_t* device, const char* restrict addr,
    int32_t val) {

}

void xcomm_melsec_1c_store_uint32(
    xcomm_melsec_device_t* device, const char* restrict addr,
    uint32_t val) {

}

void xcomm_melsec_1c_store_int64(
    xcomm_melsec_device_t* device, const char* restrict addr,
    int64_t val) {

}

void xcomm_melsec_1c_store_uint64(
    xcomm_melsec_device_t* device, const char* restrict addr,
    uint64_t val) {

}

void xcomm_melsec_1c_store_string(
    xcomm_melsec_device_t* device, const char* restrict addr,
    char* val) {

}




//////////////////////
    //static inline void
    //_melsec_1c_address_convert(const char* restrict input, char* output) {
    //    int plen = 0;
    //    while (isalpha(input[plen])) {
    //        plen++;
    //    }
    //    if (plen > FRAME_1C_MAX_COMPONENT_NAME_LENGTH) {
    //        xcomm_loge("unknown component.\n");
    //        return;
    //    }
    //    int number = atoi(input + plen);
    //    int tlen = FRAME_1C_MAX_COMPONENT_ADDRESS_LENGTH;
    //    int num_digits = tlen - plen;
    //
    //    snprintf(
    //        output, tlen + 1,
    //        "%.*s%0*d",
    //        plen,
    //        input,
    //        num_digits,
    //        number);
    //}
    //
    // static inline char*
    //_melsec_1c_segment_a_build(const char* restrict addr, int points) {
    //
    //}
    //
    // static inline char* _melsec_1c_segment_c_build(const char* restrict addr)
    // {
    //}
    //
    // static inline char* _melsec_1c_segment_b_build(const char* restrict addr)
    // {
    //}
    //
    // static inline char* _melsec_1c_message_marshalling(
    //    char*           station_no,
    //    char*           plc_no,
    //    char*           instruction,
    //    const char* restrict addr,
    //    melsec_1c_component_type_t component_type,
    //    melsec_1c_operate_type_t operate_type,
    //    int points) {
    //    char* message = malloc(FRAME_1C_MAX_REQUEST_LENGTH);
    //    if (!message) {
    //        xcomm_loge("no memory.\n");
    //        return NULL;
    //    }
    //    int offset = 0;
    //    message[offset] = frame_1c_enq;
    //
    //    memcpy(message + offset, &frame_1c_enq, FRAME_1C_ENQ_LENGTH);
    //    offset += FRAME_1C_ENQ_LENGTH;
    //
    //    memcpy(
    //        message + offset,
    //        station_no,
    //        FRAME_1C_STATION_NO_LENGTH);
    //    offset += FRAME_1C_STATION_NO_LENGTH;
    //
    //    memcpy(
    //        message + offset,
    //        plc_no,
    //        FRAME_1C_PLC_NO_LENGTH);
    //    offset += FRAME_1C_PLC_NO_LENGTH;
    //
    //    memcpy(
    //        message + offset,
    //        instruction,
    //        FRAME_1C_INSTRUCTION_LENGTH);
    //    offset += FRAME_1C_INSTRUCTION_LENGTH;
    //
    //    memcpy(
    //        message + offset,
    //        &frame_1c_timewait,
    //        FRAME_1C_TIMEWAIT_LENGTH);
    //    offset += FRAME_1C_TIMEWAIT_LENGTH;
    //
    //    if (operate_type == MELSEC_1C_L_OP_TYPE) {
    //
    //    }
    //    if (operate_type == MELSEC_1C_S_OP_TYPE) {
    //
    //    }
    //}
    //
    // static inline melsec_1c_component_type_t
    //_melsec_1c_two_character_component_type_detect(const char subtype) {
    //    melsec_1c_component_type_t type = MELSEC_1C_U_COMP_TYPE;
    //    switch (subtype) {
    //    case 'S':
    //    case 'C':
    //        type = MELSEC_1C_B_COMP_TYPE;
    //        break;
    //    case 'N':
    //        type = MELSEC_1C_W_COMP_TYPE;
    //        break;
    //    }
    //    return type;
    //}
    //
    // static inline melsec_1c_component_type_t
    //_melsec_1c_component_type_detect(const char* restrict addr) {
    //    xcomm_logi("%s enter, address: %s\n", __FUNCTION__, addr);
    //
    //    melsec_1c_component_type_t type = MELSEC_1C_U_COMP_TYPE;
    //    switch (addr[COMPONENT_TYPE_INDEX]) {
    //    case 'X':
    //    case 'Y':
    //    case 'M':
    //    case 'L':
    //    case 'F':
    //    case 'B':
    //    case 'S':
    //        type = MELSEC_1C_B_COMP_TYPE;
    //        break;
    //    case 'D':
    //    case 'W':
    //    case 'R':
    //        type = MELSEC_1C_W_COMP_TYPE;
    //        break;
    //    case 'T':
    //    case 'C':
    //        type = _melsec_1c_two_character_component_type_detect(
    //            addr[COMPONENT_SUBTYPE_INDEX]);
    //        break;
    //    }
    //    xcomm_logi(
    //        "%s leave, component type is %s.\n",
    //        __FUNCTION__,
    //        (type == MELSEC_1C_U_COMP_TYPE)
    //            ? "unknown"
    //            : ((type == MELSEC_1C_B_COMP_TYPE) ? "bit" : "word"));
    //    return type;
    //}


//#define COMPONENT_TYPE_INDEX 0
//#define COMPONENT_SUBTYPE_INDEX 1
//
//#define FRAME_1C_CHARS_PER_BIT_POINT 1
//#define FRAME_1C_CHARS_PER_WORD_POINT 4
//#define FRAME_1C_MAX_COMPONENT_NAME_LENGTH 2
//#define FRAME_1C_MAX_REQUEST_LENGTH 64
//#define FRAME_1C_MAX_RESPONSE_LENGTH 64
//
//const uint8_t frame_1c_enq = 0x05;
//const uint8_t frame_1c_stx = 0x02;
//const uint8_t frame_1c_etx = 0x03;
//const uint8_t frame_1c_nak = 0x15;
//const uint8_t frame_1c_timewait = 0x46;
//
//const uint8_t frame_1c_batch_br_instruction[] = {0x45, 0x52};
//const uint8_t frame_1c_batch_wr_instruction[] = {0x57, 0x52};
//const uint8_t frame_1c_batch_bw_instruction[] = {0x42, 0x57};
//const uint8_t frame_1c_batch_ww_instruction[] = {0x57, 0x57};
//
//typedef enum melsec_1c_operate_type_e   melsec_1c_operate_type_t;
//typedef enum melsec_1c_component_type_e melsec_1c_component_type_t;
//
//enum melsec_1c_component_type_e {
//    MELSEC_1C_U_COMP_TYPE,
//    MELSEC_1C_B_COMP_TYPE,
//    MELSEC_1C_W_COMP_TYPE,
//};
//
//enum melsec_1c_operate_type_e {
//    MELSEC_1C_U_OP_TYPE,
//    MELSEC_1C_L_OP_TYPE,
//    MELSEC_1C_S_OP_TYPE,
//};