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

#include "xcomm/xcomm-serial-module.h"

#define FRAME_1C_STATION_NO_LENGTH  2
#define FRAME_1C_PLC_NO_LENGTH      2
#define FRAME_1C_INSTRUCTION_LENGTH 2
#define FRAME_1C_CHECKSUM_LENGTH    2
#define FRAME_1C_ERRCODE_LENGTH     2

extern bool     xcomm_melsec_1c_bool_load(xcomm_serial_t* serialptr, char* station_no, char* plc_no, const char* restrict addr);
extern int8_t   xcomm_melsec_1c_int8_load(xcomm_serial_t* serialptr, char* station_no, char* plc_no, const char* restrict addr);
extern uint8_t  xcomm_melsec_1c_uint8_load(xcomm_serial_t* serialptr, char* station_no, char* plc_no, const char* restrict addr);
extern int16_t  xcomm_melsec_1c_int16_load(xcomm_serial_t* serialptr, char* station_no, char* plc_no, const char* restrict addr);
extern uint16_t xcomm_melsec_1c_uint16_load(xcomm_serial_t* serialptr, char* station_no, char* plc_no, const char* restrict addr);
extern int32_t  xcomm_melsec_1c_int32_load(xcomm_serial_t* serialptr, char* station_no, char* plc_no, const char* restrict addr);
extern uint32_t xcomm_melsec_1c_uint32_load(xcomm_serial_t* serialptr, char* station_no, char* plc_no, const char* restrict addr);
extern int64_t  xcomm_melsec_1c_int64_load(xcomm_serial_t* serialptr, char* station_no, char* plc_no, const char* restrict addr);
extern uint64_t xcomm_melsec_1c_uint64_load(xcomm_serial_t* serialptr, char* station_no, char* plc_no, const char* restrict addr);
extern const char* xcomm_melsec_1c_string_load(xcomm_serial_t* serialptr, char* station_no, char* plc_no, const char* restrict addr);

extern void xcomm_melsec_1c_bool_store(xcomm_serial_t* serialptr, char* station_no, char* plc_no, const char* restrict addr, bool val);
extern void xcomm_melsec_1c_int8_store(xcomm_serial_t* serialptr, char* station_no, char* plc_no, const char* restrict addr, int8_t val);
extern void xcomm_melsec_1c_uint8_store(xcomm_serial_t* serialptr, char* station_no, char* plc_no, const char* restrict addr, uint8_t val);
extern void xcomm_melsec_1c_int16_store(xcomm_serial_t* serialptr, char* station_no, char* plc_no, const char* restrict addr, int16_t val);
extern void xcomm_melsec_1c_uint16_store(xcomm_serial_t* serialptr, char* station_no, char* plc_no, const char* restrict addr, uint16_t val);
extern void xcomm_melsec_1c_int32_store(xcomm_serial_t* serialptr, char* station_no, char* plc_no, const char* restrict addr, int32_t val);
extern void xcomm_melsec_1c_uint32_store(xcomm_serial_t* serialptr, char* station_no, char* plc_no, const char* restrict addr, uint32_t val);
extern void xcomm_melsec_1c_int64_store(xcomm_serial_t* serialptr, char* station_no, char* plc_no, const char* restrict addr, int64_t val);
extern void xcomm_melsec_1c_uint64_store(xcomm_serial_t* serialptr, char* station_no, char* plc_no, const char* restrict addr, uint64_t val);
extern void xcomm_melsec_1c_string_store(xcomm_serial_t* serialptr, char* station_no, char* plc_no, const char* restrict addr, const char* val);
