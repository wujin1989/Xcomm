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

#include <stdbool.h>

#define xcomm_logd(logger, ...)    xcomm_logger_log(logger, XCOMM_LOGGER_LEVEL_DEBUG, __FILE__, __LINE__, __VA_ARGS__)
#define xcomm_logi(logger, ...)    xcomm_logger_log(logger, XCOMM_LOGGER_LEVEL_INFO,  __FILE__, __LINE__, __VA_ARGS__)
#define xcomm_logw(logger, ...)    xcomm_logger_log(logger, XCOMM_LOGGER_LEVEL_WARN,  __FILE__, __LINE__, __VA_ARGS__)
#define xcomm_loge(logger, ...)    xcomm_logger_log(logger, XCOMM_LOGGER_LEVEL_ERROR, __FILE__, __LINE__, __VA_ARGS__)

typedef enum xcomm_logger_level_e xcomm_logger_level_t;
typedef struct xcomm_logger_s     xcomm_logger_t;

enum xcomm_logger_level_e {
    XCOMM_LOGGER_LEVEL_DEBUG = 1,
    XCOMM_LOGGER_LEVEL_INFO  = 2,
    XCOMM_LOGGER_LEVEL_WARN  = 3,
    XCOMM_LOGGER_LEVEL_ERROR = 4,
};

struct xcomm_logger_s {
    const char* restrict filename;
    bool                 async;
    xcomm_logger_level_t level;
    void*                opaque;
};

extern void xcomm_logger_init(xcomm_logger_t* logger);
extern void xcomm_logger_destroy(xcomm_logger_t* logger);
extern void xcomm_logger_callback(xcomm_logger_t* logger, void (*callback)(xcomm_logger_level_t level, const char* restrict msg));
