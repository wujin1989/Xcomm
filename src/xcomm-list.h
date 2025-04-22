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

#include "xcomm-types.h"

#define xcomm_list_data(x, t, m) ((t *)((char *)(x) - offsetof(t, m)))

extern void xcomm_list_init(xcomm_list_t* list);
extern void xcomm_list_insert_head(xcomm_list_t* list, xcomm_list_node_t* node);
extern void xcomm_list_insert_tail(xcomm_list_t* list, xcomm_list_node_t* node);
extern void xcomm_list_remove(xcomm_list_node_t* node);
extern bool xcomm_list_empty(xcomm_list_t* list);
extern xcomm_list_node_t* xcomm_list_head(xcomm_list_t* list);
extern xcomm_list_node_t* xcomm_list_tail(xcomm_list_t* list);
extern xcomm_list_node_t* xcomm_list_next(xcomm_list_node_t* node);
extern xcomm_list_node_t* xcomm_list_prev(xcomm_list_node_t* node);
extern xcomm_list_node_t* xcomm_list_sentinel(xcomm_list_t* list);