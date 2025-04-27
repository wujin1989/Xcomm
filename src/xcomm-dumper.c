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

#include <stdio.h>

#include "xcomm-logger.h"
#include "xcomm/xcomm-dumper.h"

void xcomm_dumper_init(xcomm_dumper_config_t* config) {
    switch (config->mode) {
    case XCOMM_DUMPER_MODE_FILE:
        xcomm_logger_init(config->filename, XCOMM_LOGGER_LEVEL_DEBUG, true);
        break;
    case XCOMM_DUMPER_MODE_CALLBACK:
        xcomm_logger_set_callback((void (*)(xcomm_logger_level_t level, const char* restrict msg))
                config->callback);
        xcomm_logger_init(NULL, XCOMM_LOGGER_LEVEL_DEBUG, true);
        break;
    default:
        break;
    }
}

void xcomm_dumper_destroy(void) {
    xcomm_logger_destroy();
}