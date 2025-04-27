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
#include "xcomm-utils.h"

#define BUFSIZE 4096

typedef struct builtin_logger_s       builtin_logger_t;
typedef struct builtin_printer_ctx_s  builtin_printer_ctx_t;

struct builtin_printer_ctx_s {
    xcomm_logger_level_t level;
    char                 message[];
};

struct builtin_logger_s {
    bool                 async;
    xcomm_logger_level_t level;
    FILE*                file;
    mtx_t                mtx;
    xcomm_thrdpool_t     thrdpool;
    atomic_bool          initialized;
    void (*callback)(xcomm_logger_level_t level, const char* restrict msg);
};

static builtin_logger_t logger;
static const char* levels[] = {"DEBUG", "INFO", "WARN", "ERROR"};

static inline void _printer(void* param) {
    builtin_printer_ctx_t* ctx = param;

    if (logger.callback) {
        logger.callback(ctx->level, ctx->message);
    } else {
        fprintf(logger.file, "%s", ctx->message);
        fflush(logger.file);
    }
    free(ctx);
}

static int _buffer_marshall(char* buf, size_t buflen,
    xcomm_logger_level_t level,
    const char* restrict file,
    int line,
    const char* restrict fmt,
    va_list v) {
    int ret;
    struct tm tm;
    struct timespec tsc;

    if (logger.callback) {
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
    ret += xcomm_io_vsprintf(buf + ret, buflen - ret, fmt, v);
    ret++;
    return ret;
}

static void _sync_log(
    xcomm_logger_level_t level,
    const char* restrict file,
    int line,
    const char* restrict fmt,
    va_list v) {
    char buf[BUFSIZE] = {0};

    mtx_lock(&logger.mtx);
    int ret = _buffer_marshall(buf, sizeof(buf), level, file, line, fmt, v);
    builtin_printer_ctx_t* ctx = malloc(sizeof(builtin_printer_ctx_t) + ret);
    if (ctx) {
        memcpy(ctx->message, buf, ret);
        ctx->level = level;
        _printer(ctx);
    }
    mtx_unlock(&logger.mtx);
}

static void _async_log(
    xcomm_logger_level_t level,
    const char* restrict file,
    int line,
    const char* restrict fmt,
    va_list v) {
    char buf[BUFSIZE] = {0};

    mtx_lock(&logger.mtx);
    int ret = _buffer_marshall(buf, sizeof(buf), level, file, line, fmt, v);
    builtin_printer_ctx_t* ctx = malloc(sizeof(builtin_printer_ctx_t) + ret);
    if (ctx) {
        memcpy(ctx->message, buf, ret);
        ctx->level = level;
        xcomm_thrdpool_post(&logger.thrdpool, _printer, ctx);
    }
    mtx_unlock(&logger.mtx);
}

void xcomm_logger_init(
    const char* restrict filename, xcomm_logger_level_t level, bool async) {
    if (!atomic_load(&logger.initialized)) {
        atomic_store(&logger.initialized, true);

        if (level > XCOMM_LOGGER_LEVEL_ERROR ||
            level < XCOMM_LOGGER_LEVEL_DEBUG) {
            logger.level = XCOMM_LOGGER_LEVEL_INFO;
        } else {
            logger.level = level;
        }
        if (filename) {
            logger.file = xcomm_io_fopen(filename, "a+");
        } else {
            logger.file = stdout;
        }
        if (async) {
            xcomm_thrdpool_init(&logger.thrdpool, 1);
            logger.async = true;
        } else {
            logger.async = false;
        }
        mtx_init(&logger.mtx, mtx_plain);
    }
}

void xcomm_logger_destroy(void) {
    if (atomic_load(&logger.initialized)) {
        atomic_store(&logger.initialized, false);

        fclose(logger.file);
        if (logger.async) {
            xcomm_thrdpool_destroy(&logger.thrdpool);
        }
        mtx_destroy(&logger.mtx);
    }
}

void xcomm_logger_log(
    xcomm_logger_level_t level,
    const char* restrict file,
    int line,
    const char* restrict fmt,
    ...) {
    if (!atomic_load(&logger.initialized)) {
        return;
    }
    char* p = strrchr(file, XCOMM_PATH_SEPARATOR);

    if (logger.level > level) {
        return;
    }
    if (logger.async) {
        va_list v;
        va_start(v, fmt);
        if (!p) {
            _async_log(level, file, line, fmt, v);
        }
        if (p) {
            _async_log(level, ++p, line, fmt, v);
        }
        va_end(v);
    } else {
        va_list v;
        va_start(v, fmt);
        if (!p) {
            _sync_log(level, file, line, fmt, v);
        }
        if (p) {
            _sync_log(level, ++p, line, fmt, v);
        }
        va_end(v);
    }
}

void xcomm_logger_set_callback(
    void (*callback)(xcomm_logger_level_t level, const char* restrict msg)) {
    logger.callback = callback;
}