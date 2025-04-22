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

#include "xcomm-utils.h"
#include "platform/platform-utils.h"

int xcomm_utils_cpus(void) {
    return platform_utils_cpus();
}

xcomm_tid_t xcomm_utils_gettid(void) {
    return platform_utils_gettid();
}

xcomm_pid_t xcomm_utils_getpid(void) {
    return platform_utils_getpid();
}

int xcomm_utils_byteorder(void) {
    /**
     * 0 if the system is big-endian.
     * 1 if the system is little-endian.
     */
    return (*((unsigned char*)(&(unsigned short){0x01})));
}

int xcomm_utils_rand(int min, int max) {
    static unsigned int seed = 0;
    if (seed == 0) {
        seed = (unsigned int)time(NULL);
        srand(seed);
    }
    return min + (int)((double)((double)(max) - (min) + 1.0) *
                       (rand() / ((RAND_MAX) + 1.0)));
}