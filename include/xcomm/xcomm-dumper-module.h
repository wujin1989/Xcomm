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

_Pragma("once")

typedef enum xcomm_dumper_mode_s     xcomm_dumper_mode_t;
typedef struct xcomm_dumper_config_s xcomm_dumper_config_t;
typedef enum xcomm_dumper_level_e    xcomm_dumper_level_t;
typedef struct xcomm_dumper_module_s xcomm_dumper_module_t;

enum xcomm_dumper_mode_s {
    XCOMM_DUMPER_MODE_FILE,
    XCOMM_DUMPER_MODE_CALLBACK,
};

enum xcomm_dumper_level_e {
    XCOMM_DUMPER_LEVEL_DEBUG,
    XCOMM_DUMPER_LEVEL_INFO,
    XCOMM_DUMPER_LEVEL_WARN,
    XCOMM_DUMPER_LEVEL_ERROR,
};

struct xcomm_dumper_config_s {
    union {
        const char* restrict filename;
        void (*callback)(xcomm_dumper_level_t level, const char* restrict msg);
    };
    xcomm_dumper_mode_t  mode;
    xcomm_dumper_level_t level;
};

struct xcomm_dumper_module_s {
    const char* restrict name;
    void (*init)(xcomm_dumper_config_t* config);
    void (*destroy)(void);
};

extern xcomm_dumper_module_t xcomm_dumper;
