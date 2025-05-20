/** Copyright (c) 2025, Wu Jin <wujin.developer@gmail.com>
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

#include <stdio.h>

#include "xcomm-logger.h"
#include "xcomm-dumper.h"
#include "deprecated/c11-threads.h"

typedef struct dumper_context_s dumper_context_t;

struct dumper_context_s {
    void (*callback)(xcomm_dumper_level_t level, const char* restrict msg);
};

static thread_local dumper_context_t ctx;

static const xcomm_logger_level_t d2l_level_map[] = {
    [XCOMM_DUMPER_LEVEL_DEBUG] = XCOMM_LOGGER_LEVEL_DEBUG,
    [XCOMM_DUMPER_LEVEL_INFO]  = XCOMM_LOGGER_LEVEL_INFO,
    [XCOMM_DUMPER_LEVEL_WARN]  = XCOMM_LOGGER_LEVEL_WARN,
    [XCOMM_DUMPER_LEVEL_ERROR] = XCOMM_LOGGER_LEVEL_ERROR,
};

static const xcomm_dumper_level_t l2d_level_map[] = {
    [XCOMM_LOGGER_LEVEL_DEBUG] = XCOMM_DUMPER_LEVEL_DEBUG,
    [XCOMM_LOGGER_LEVEL_INFO]  = XCOMM_DUMPER_LEVEL_INFO,
    [XCOMM_LOGGER_LEVEL_WARN]  = XCOMM_DUMPER_LEVEL_WARN,
    [XCOMM_LOGGER_LEVEL_ERROR] = XCOMM_DUMPER_LEVEL_ERROR,
};

static void
_dumper_logger_callback(xcomm_logger_level_t level, const char* restrict msg) {
    ctx.callback(l2d_level_map[level], msg);
}

void xcomm_dumper_init(xcomm_dumper_config_t* config) {
    ctx.callback = config->callback;

    switch (config->mode) {
    case XCOMM_DUMPER_MODE_FILE:
        xcomm_logger_init(config->filename, d2l_level_map[config->level], true);
        break;
    case XCOMM_DUMPER_MODE_CALLBACK:
        xcomm_logger_set_callback(_dumper_logger_callback);
        xcomm_logger_init(NULL, d2l_level_map[config->level], true);
        break;
    default:
        break;
    }
}

void xcomm_dumper_destroy(void) {
    xcomm_logger_destroy();
}