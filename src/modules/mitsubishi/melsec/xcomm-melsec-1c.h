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

#define FRAME_1C_STATION_NO_LENGTH  2
#define FRAME_1C_PLC_NO_LENGTH      2
#define FRAME_1C_INSTRUCTION_LENGTH 2
#define FRAME_1C_CHECKSUM_LENGTH    2
#define FRAME_1C_ERRCODE_LENGTH     2

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

