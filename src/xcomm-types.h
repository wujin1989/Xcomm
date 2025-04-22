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

#include "platform/platform-types.h"

#define XCOMM_SO_ERROR_EAGAIN                   PLATFORM_SO_ERROR_EAGAIN
#define XCOMM_SO_ERROR_EWOULDBLOCK              PLATFORM_SO_ERROR_EWOULDBLOCK
#define XCOMM_SO_ERROR_ECONNRESET               PLATFORM_SO_ERROR_ECONNRESET
#define XCOMM_SO_ERROR_ETIMEDOUT                PLATFORM_SO_ERROR_ETIMEDOUT
#define XCOMM_SO_ERROR_INVALID_SOCKET           PLATFORM_SO_ERROR_INVALID_SOCKET
#define XCOMM_SO_ERROR_SOCKET_ERROR             PLATFORM_SO_ERROR_SOCKET_ERROR

#define XCOMM_MAX_PROCESS_EVENTS                PLATFORM_MAX_PROCESS_EVENTS

typedef platform_tid_t       xcomm_tid_t;
typedef platform_pid_t       xcomm_pid_t;
typedef platform_pollfd_t    xcomm_pollfd_t;
typedef platform_sock_t      xcomm_sock_t;
typedef platform_pollevent_t xcomm_pollevent_t;

typedef struct xcomm_heap_s        xcomm_heap_t;
typedef struct xcomm_heap_node_s   xcomm_heap_node_t;
typedef struct xcomm_list_node_s   xcomm_list_node_t;
typedef struct xcomm_list_node_s   xcomm_list_t;
typedef struct xcomm_list_node_s   xcomm_queue_t;
typedef struct xcomm_list_node_s   xcomm_queue_node_t;
typedef union xcomm_rbtree_key_u   xcomm_rbtree_key_t;
typedef struct xcomm_rbtree_node_s xcomm_rbtree_node_t;
typedef struct xcomm_rbtree_s      xcomm_rbtree_t;
typedef struct xcomm_ringbuf_s     xcomm_ringbuf_t;
typedef struct xcomm_sha1_s        xcomm_sha1_t;
typedef struct xcomm_sha256_s      xcomm_sha256_t;

struct xcomm_heap_node_s {
    struct xcomm_heap_node_s* left;
    struct xcomm_heap_node_s* right;
    struct xcomm_heap_node_s* parent;
};

struct xcomm_heap_s {
    struct xcomm_heap_node_s* heap_min;
    size_t                    heap_nelts;
    /* a < b return positive that means min-heap */
    /* a > b return positive, means max-heap */
    int (*compare)(xcomm_heap_node_t* a, xcomm_heap_node_t* b);
};

struct xcomm_list_node_s {
    struct xcomm_list_node_s* prev;
    struct xcomm_list_node_s* next;
};

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

struct xcomm_ringbuf_s {
    char*    buf;
    uint32_t wpos; /* write pos */
    uint32_t rpos; /* read pos */
    uint32_t mask; /* mask */
    uint32_t esz;  /* entry size */
};

struct xcomm_sha1_s {
    uint32_t state[5];
    size_t   count[2];
    uint8_t  buffer[64];
};

struct xcomm_sha256_s {
    uint8_t  data[64];
    uint32_t datalen;
    uint64_t bitlen;
    uint32_t state[8];
};