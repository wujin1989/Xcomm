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

#include <stdio.h>
#include <stdbool.h>

typedef struct xcomm_s                 xcomm_t;
typedef struct xcomm_scene_s           xcomm_scene_t;
typedef struct xcomm_melsec_1c_scene_s xcomm_melsec_1c_scene_t;
typedef struct xcomm_melsec_3c_scene_s xcomm_melsec_3c_scene_t;
typedef struct xcomm_melsec_3e_scene_s xcomm_melsec_3e_scene_t;

const char* xcomm_scenes[] = {"melsec-1c", "melsec-3c", "melsec-3e"};

struct xcomm_melsec_1c_scene_s {
    int a;
};

struct xcomm_melsec_3c_scene_s {
    int a;
};

struct xcomm_melsec_3e_scene_s {
    int a;
};

struct xcomm_scene_s {
    union {
        xcomm_melsec_1c_scene_t mc_1c;
        xcomm_melsec_3c_scene_t mc_3c;
        xcomm_melsec_3e_scene_t mc_3e;
    };
    char* restrict name;
};

struct xcomm_s {
    xcomm_scene_t scene;
    bool          logging;
};

extern xcomm_t* xcomm_create(xcomm_scene_t scene);
extern void     xcomm_destroy(xcomm_t* restrict xcomm);
