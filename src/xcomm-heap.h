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

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define xcomm_heap_data(x, t, m) ((t *)((char *)(x) - offsetof(t, m)))

typedef struct xcomm_heap_s      xcomm_heap_t;
typedef struct xcomm_heap_node_s xcomm_heap_node_t;

struct xcomm_heap_node_s {
    struct xcomm_heap_node_s* left;
    struct xcomm_heap_node_s* right;
    struct xcomm_heap_node_s* parent;
};

struct xcomm_heap_s {
    struct xcomm_heap_node_s* heap_min;
    size_t                    heap_nelts;
    /* a < b return positive that means min-heap */
    /* a > b return positive, means max-heap     */
    int (*compare)(xcomm_heap_node_t* a, xcomm_heap_node_t* b);
};

extern void xcomm_heap_init(xcomm_heap_t* heap, int (*cmp)(xcomm_heap_node_t* a, xcomm_heap_node_t* b));
extern void xcomm_heap_insert(xcomm_heap_t* heap, xcomm_heap_node_t* node);
extern void xcomm_heap_remove(xcomm_heap_t* heap, xcomm_heap_node_t* node);
extern void xcomm_heap_dequeue(xcomm_heap_t* heap);
extern bool xcomm_heap_empty(xcomm_heap_t* heap);
extern xcomm_heap_node_t* xcomm_heap_min(xcomm_heap_t* heap);