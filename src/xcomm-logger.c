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

#include "xcomm-logger.h"
#include "xcomm-io.h"
#include "xcomm-thrdpool.h"
#include "xcomm-time.h"
#include "xcomm-types.h"
#include "xcomm-utils.h"

#define BUFSIZE 4096

typedef struct internal_logger_s      internal_logger_t;
typedef struct internal_printer_ctx_s internal_printer_ctx_t;

struct internal_printer_ctx_s {
    xcomm_logger_level_t level;
    internal_logger_t*   internal;
    char                 message[];
};

struct internal_logger_s {
    FILE*            file;
    mtx_t            mtx;
    xcomm_thrdpool_t thrdpool;
    void (*callback)(xcomm_logger_level_t level, const char* restrict msg);
};

static const char* levels[] = {"DEBUG", "INFO", "WARN", "ERROR"};

static inline void _printer(void* param) {
    internal_printer_ctx_t* ctx = param;
    internal_logger_t*      internal = ctx->internal;

    if (internal->callback) {
        internal->callback(ctx->level, ctx->message);
    } else {
        fprintf(internal->file, "%s", ctx->message);
        fflush(internal->file);
    }
    free(ctx);
}

static inline int _buffer_marshall(
    internal_logger_t*   internal,
    char*                buf,
    xcomm_logger_level_t level,
    const char* restrict file,
    int line,
    const char* restrict fmt,
    va_list v) {
    int             ret;
    struct timespec tsc;
    struct tm       tm;

    if (internal->callback) {
        ret = sprintf(buf, "%s:%d ", file, line);
    } else {
        timespec_get(&tsc, TIME_UTC);
        xcomm_time_localtime(&tsc.tv_sec, &tm);
        ret = sprintf(
            buf,
            "%04d-%02d-%02d %02d:%02d:%02d.%03d %8d %5s %s:%d ",
            tm.tm_year + 1900,
            tm.tm_mon + 1,
            tm.tm_mday,
            tm.tm_hour,
            tm.tm_min,
            tm.tm_sec,
            (int)(tsc.tv_nsec / 1000000UL),
            (int)xcomm_utils_gettid(),
            levels[level],
            file,
            line);
    }
    if (ret > sizeof(buf)) {
        abort();
    }
    ret += xcomm_io_vsprintf(buf + ret, sizeof(buf) - ret, fmt, v);
    ret++;
    return ret;
}

static inline void _sync_log(
    internal_logger_t*   internal,
    xcomm_logger_level_t level,
    const char* restrict file,
    int line,
    const char* restrict fmt,
    va_list v) {
    char buf[BUFSIZE] = {0};

    mtx_lock(&internal->mtx);
    int ret = _buffer_marshall(internal, buf, level, file, line, fmt, v);
    internal_printer_ctx_t* ctx = malloc(sizeof(internal_printer_ctx_t) + ret);
    if (ctx) {
        memcpy(ctx->message, buf, ret);
        ctx->internal = internal;
        ctx->level = level;
        _printer(ctx);
    }
    mtx_unlock(&internal->mtx);
}

static inline void _async_log(
    internal_logger_t*   internal,
    xcomm_logger_level_t level,
    const char* restrict file,
    int line,
    const char* restrict fmt,
    va_list v) {
    char buf[BUFSIZE] = {0};

    mtx_lock(&internal->mtx);
    int ret = _buffer_marshall(internal, buf, level, file, line, fmt, v);
    internal_printer_ctx_t* ctx = malloc(sizeof(internal_printer_ctx_t) + ret);
    if (ctx) {
        memcpy(ctx->message, buf, ret);
        ctx->internal = internal;
        ctx->level = level;
        xcomm_thrdpool_post(&internal->thrdpool, _printer, ctx);
    }
    mtx_unlock(&internal->mtx);
}

void xcomm_logger_init(xcomm_logger_t* logger) {
    logger->opaque = malloc(sizeof(internal_logger_t));
    if (!logger->opaque) {
        return;
    }
    internal_logger_t* internal = logger->opaque;

    if (logger->level > XCOMM_LOGGER_LEVEL_ERROR ||
        logger->level < XCOMM_LOGGER_LEVEL_DEBUG) {
        logger->level = XCOMM_LOGGER_LEVEL_DEBUG;
    }
    if (logger->filename) {
        internal->file = xcomm_io_fopen(logger->filename, "a+");
    } else {
        internal->file = stdout;
    }
    if (logger->async) {
        xcomm_thrdpool_init(&internal->thrdpool, 1);
    }
    mtx_init(&internal->mtx, mtx_plain);
}

void xcomm_logger_destroy(xcomm_logger_t* logger) {
    internal_logger_t* internal = logger->opaque;

    fclose(internal->file);
    if (logger->async) {
        xcomm_thrdpool_destroy(&internal->thrdpool);
    }
    mtx_destroy(&internal->mtx);
    free(internal);
}

void xcomm_logger_log(
    xcomm_logger_t*      logger,
    xcomm_logger_level_t level,
    const char* restrict file,
    int line,
    const char* restrict fmt,
    ...) {
    internal_logger_t* internal = logger->opaque;
    char* p = strrchr(file, XCOMM_PATH_SEPARATOR);

    if (logger->level > level) {
        return;
    }
    if (logger->async) {
        va_list v;
        va_start(v, fmt);
        if (!p) {
            _async_log(internal, level, file, line, fmt, v);
        }
        if (p) {
            _async_log(internal, level, ++p, line, fmt, v);
        }
        va_end(v);
    } else {
        va_list v;
        va_start(v, fmt);
        if (!p) {
            _sync_log(internal, level, file, line, fmt, v);
        }
        if (p) {
            _sync_log(internal, level, ++p, line, fmt, v);
        }
        va_end(v);
    }
}

void xcomm_logger_callback(
    xcomm_logger_t* logger,
    void (*callback)(xcomm_logger_level_t level, const char* restrict msg)) {
    internal_logger_t* internal = logger->opaque;
    internal->callback = callback;
}