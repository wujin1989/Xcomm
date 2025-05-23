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

#define xcomm_rbtree_builtin_keycmp(k1, k2)                                    \
    _Generic(                                                                  \
        (k1),                                                                  \
          int8_t: xcomm_builtin_keycmp_i8,                                     \
         int16_t: xcomm_builtin_keycmp_i16,                                    \
         int32_t: xcomm_builtin_keycmp_i32,                                    \
         int64_t: xcomm_builtin_keycmp_i64,                                    \
         uint8_t: xcomm_builtin_keycmp_u8,                                     \
        uint16_t: xcomm_builtin_keycmp_u16,                                    \
        uint32_t: xcomm_builtin_keycmp_u32,                                    \
        uint64_t: xcomm_builtin_keycmp_u64,                                    \
           char*: xcomm_builtin_keycmp_str)(                                   \
        (xcomm_rbtree_key_t*)(k1), (xcomm_rbtree_key_t*)(k2))

#define xcomm_rbtree_data ((t *)((char *)(x) - offsetof(t, m)))

typedef union xcomm_rbtree_key_u   xcomm_rbtree_key_t;
typedef struct xcomm_rbtree_node_s xcomm_rbtree_node_t;
typedef struct xcomm_rbtree_s      xcomm_rbtree_t;

union xcomm_rbtree_key_u {
    char*    str;
    int8_t   i8;
    int16_t  i16;
    int32_t  i32;
    int64_t  i64;
    uint8_t  u8;
    uint16_t u16;
    uint32_t u32;
    uint64_t u64;
    void*    ptr;
};

struct xcomm_rbtree_node_s {
    xcomm_rbtree_key_t          key;
    struct xcomm_rbtree_node_s* parent;
    struct xcomm_rbtree_node_s* right;
    struct xcomm_rbtree_node_s* left;
    char                        color;
};

struct xcomm_rbtree_s {
    xcomm_rbtree_node_t* root;
    int (*compare)(xcomm_rbtree_key_t* k1, xcomm_rbtree_key_t* k2);
};

extern void xcomm_rbtree_init(xcomm_rbtree_t* tree, int (*keycmp)(xcomm_rbtree_key_t*, xcomm_rbtree_key_t*));
extern void xcomm_rbtree_insert(xcomm_rbtree_t* tree, xcomm_rbtree_node_t* node);
extern void xcomm_rbtree_erase(xcomm_rbtree_t* tree, xcomm_rbtree_node_t* node);
extern bool xcomm_rbtree_empty(xcomm_rbtree_t* tree);
extern xcomm_rbtree_node_t* xcomm_rbtree_find(xcomm_rbtree_t* tree, xcomm_rbtree_key_t key);
extern xcomm_rbtree_node_t* xcomm_rbtree_next(xcomm_rbtree_node_t* node);
extern xcomm_rbtree_node_t* xcomm_rbtree_prev(xcomm_rbtree_node_t* node);
extern xcomm_rbtree_node_t* xcomm_rbtree_first(xcomm_rbtree_t* tree);
extern xcomm_rbtree_node_t* xcomm_rbtree_last(xcomm_rbtree_t* tree);

/* used by internal */
extern int xcomm_builtin_keycmp_i8(xcomm_rbtree_key_t* k1, xcomm_rbtree_key_t* k2);
extern int xcomm_builtin_keycmp_i16(xcomm_rbtree_key_t* k1, xcomm_rbtree_key_t* k2);
extern int xcomm_builtin_keycmp_i32(xcomm_rbtree_key_t* k1, xcomm_rbtree_key_t* k2);
extern int xcomm_builtin_keycmp_i64(xcomm_rbtree_key_t* k1, xcomm_rbtree_key_t* k2);
extern int xcomm_builtin_keycmp_u8(xcomm_rbtree_key_t* k1, xcomm_rbtree_key_t* k2);
extern int xcomm_builtin_keycmp_u16(xcomm_rbtree_key_t* k1, xcomm_rbtree_key_t* k2);
extern int xcomm_builtin_keycmp_u32(xcomm_rbtree_key_t* k1, xcomm_rbtree_key_t* k2);
extern int xcomm_builtin_keycmp_u64(xcomm_rbtree_key_t* k1, xcomm_rbtree_key_t* k2);
extern int xcomm_builtin_keycmp_str(xcomm_rbtree_key_t* k1, xcomm_rbtree_key_t* k2);