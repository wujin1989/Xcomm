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

#define INTERNAL_STATION_NO_LENGTH  2
#define INTERNAL_PLC_NO_LENGTH      2
#define INTERNAL_INSTRUCTION_LENGTH 2
#define INTERNAL_CHECKSUM_LENGTH    2
#define INTERNAL_ERRCODE_LENGTH     2

typedef struct internal_melsec_1c_fmt01_req_s         internal_melsec_1c_fmt01_req_t;
typedef union internal_melsec_1c_fmt01_rsp_u          internal_melsec_1c_fmt01_rsp_t;
typedef struct internal_melsec_1c_fmt01_rsp_success_s internal_melsec_1c_fmt01_rsp_success_t;
typedef struct internal_melsec_1c_fmt01_rsp_failure_s internal_melsec_1c_fmt01_rsp_failure_t;

struct internal_melsec_1c_fmt01_req_s {
    char enq;
    char timeout;
    char station_no[INTERNAL_STATION_NO_LENGTH];
    char instruction[INTERNAL_INSTRUCTION_LENGTH];
    char plc_no[INTERNAL_PLC_NO_LENGTH];
    char checksum[INTERNAL_CHECKSUM_LENGTH];
    char segment[];
};

union internal_melsec_1c_fmt01_rsp_u {
    internal_melsec_1c_fmt01_rsp_success_t success;
    internal_melsec_1c_fmt01_rsp_failure_t failure;
};

struct internal_melsec_1c_fmt01_rsp_success_s {
    char ack;
    char station_no[INTERNAL_STATION_NO_LENGTH];
    char instruction[INTERNAL_INSTRUCTION_LENGTH];
};

struct internal_melsec_1c_fmt01_rsp_failure_s {
    char nak;
    char station_no[INTERNAL_STATION_NO_LENGTH];
    char instruction[INTERNAL_INSTRUCTION_LENGTH];
    char errcode[INTERNAL_ERRCODE_LENGTH];
};