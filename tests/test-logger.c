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

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "xcomm-logger.h"
#include "deprecated/c11-threads.h"

static const char*          TEST_FILE = "test.log";
static int                  callback_count = 0;
static xcomm_logger_level_t last_callback_level;
static char                 last_callback_msg[1024];

static void
logger_callback(xcomm_logger_level_t level, const char* restrict msg) {
    callback_count++;
    last_callback_level = level;
    strncpy(last_callback_msg, msg, sizeof(last_callback_msg));
}

static void cleanup() {
    remove(TEST_FILE);
}

void assert_file_contains(const char* filename, const char* substr) {
    FILE* fp = fopen(filename, "r");
    assert(fp != NULL);

    char buffer[1024];
    bool found = false;
    while (fgets(buffer, sizeof(buffer), fp)) {
        if (strstr(buffer, substr) != NULL) {
            found = true;
            break;
        }
    }
    fclose(fp);
    assert(found);
}

void test_basic_logging() {
    xcomm_logger_init(TEST_FILE, XCOMM_LOGGER_LEVEL_DEBUG, false);

    const char* test_msg = "Basic test message";
    xcomm_logi("Info log: %s\n", test_msg);
    xcomm_loge("Error log: %d\n", 42);

    xcomm_logger_destroy();

    assert_file_contains(TEST_FILE, "Info log: Basic test message");
    assert_file_contains(TEST_FILE, "Error log: 42");
    cleanup();
}

void test_level_filter() {
    xcomm_logger_init(TEST_FILE, XCOMM_LOGGER_LEVEL_WARN, false);

    xcomm_logd("Debug message\n");
    xcomm_logw("Warning message\n");
    xcomm_logi("Info message\n");
    xcomm_loge("Error message\n");

    xcomm_logger_destroy();

    FILE* fp = fopen(TEST_FILE, "r");
    assert(fp != NULL);
    char buffer[1024];
    bool has_debug = false, has_warn = false;
    while (fgets(buffer, sizeof(buffer), fp)) {
        if (strstr(buffer, "Debug message"))
            has_debug = true;
        if (strstr(buffer, "Warning message"))
            has_warn = true;
    }
    fclose(fp);

    assert(!has_debug);
    assert(has_warn);
    cleanup();
}

void test_async_logging() {
    xcomm_logger_init(TEST_FILE, XCOMM_LOGGER_LEVEL_DEBUG, true);

    for (int i = 0; i < 1000; i++) {
        xcomm_logi("Async message %d\n", i);
    }
    thrd_sleep(&(struct timespec){.tv_sec = 0, .tv_nsec = 100000000}, NULL);
    xcomm_logger_destroy();

    FILE* fp = fopen(TEST_FILE, "r");
    assert(fp != NULL);

    int  line_count = 0;
    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), fp)) {
        line_count++;
    }
    fclose(fp);
    assert(line_count == 1000);
    cleanup();
}

void test_callback_feature() {
    xcomm_logger_init(TEST_FILE, XCOMM_LOGGER_LEVEL_DEBUG, false);
    xcomm_logger_set_callback(logger_callback);

    const char* warn_msg = "Callback test warning";
    xcomm_logw("%s\n", warn_msg);

    assert(callback_count == 1);
    assert(last_callback_level == XCOMM_LOGGER_LEVEL_WARN);
    assert(strstr(last_callback_msg, warn_msg) != NULL);

    xcomm_logger_destroy();
    cleanup();
}

int main(void) {
    test_basic_logging();
    test_level_filter();
    test_async_logging();
    test_callback_feature();
    return 0;
}