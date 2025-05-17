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

#include "xcomm-heap.h"

/* swap parent with child. child moves closer to the root, parent moves away. */
static void _heap_node_swap(
    xcomm_heap_t* heap, 
    xcomm_heap_node_t* parent, 
    xcomm_heap_node_t* child) {
    xcomm_heap_node_t* sibling;
    xcomm_heap_node_t  t;

    t = *parent;
    *parent = *child;
    *child = t;

    parent->parent = child;
    if (child->left == child) {
        child->left = parent;
        sibling = child->right;
    } else {
        child->right = parent;
        sibling = child->left;
    }
    if (sibling != NULL) {
        sibling->parent = child;
    }
    if (parent->left != NULL) {
        parent->left->parent = parent;
    }
    if (parent->right != NULL) {
        parent->right->parent = parent;
    }
    if (child->parent == NULL) {
        heap->heap_min = child;
    } else if (child->parent->left == parent) {
        child->parent->left = child;
    } else {
        child->parent->right = child;
    }
}

void xcomm_heap_init(
    xcomm_heap_t* heap,
    int (*cmp)(xcomm_heap_node_t* a, xcomm_heap_node_t* b)) {
    heap->heap_min = NULL;
    heap->heap_nelts = 0;
    heap->compare = cmp;
}

void xcomm_heap_insert(xcomm_heap_t* heap, xcomm_heap_node_t* node) {
    xcomm_heap_node_t** parent;
    xcomm_heap_node_t** child;
    size_t              path;
    size_t              n;
    size_t              k;

    node->left = NULL;
    node->right = NULL;
    node->parent = NULL;

    /* calculate the path from the root to the insertion point.  this is a min
     * heap so we always insert at the left-most free node of the bottom row.
     */
    path = 0;
    for (k = 0, n = 1 + heap->heap_nelts; n >= 2; k += 1, n /= 2) {
        path = (path << 1) | (n & 1);
    }
    /* now traverse the heap using the path we calculated in the previous step.
     */
    parent = child = &heap->heap_min;
    while (k > 0) {
        parent = child;
        if (path & 1) {
            child = &((*child)->right);
        } else {
            child = &((*child)->left);
        }
        path >>= 1;
        k -= 1;
    }
    /* insert the new node. */
    node->parent = *parent;
    *child = node;
    heap->heap_nelts += 1;
    /* walk up the tree and check at each node if the heap property holds.
     * it's a min heap so parent < child must be true.
     */
    while (node->parent != NULL && heap->compare(node, node->parent)) {
        _heap_node_swap(heap, node->parent, node);
    }
}

void xcomm_heap_remove(xcomm_heap_t* heap, xcomm_heap_node_t* node) {
    xcomm_heap_node_t*  smallest;
    xcomm_heap_node_t** max;
    xcomm_heap_node_t*  child;
    size_t              path;
    size_t              k;
    size_t              n;

    if (heap->heap_nelts == 0) {
        return;
    }
    /* calculate the path from the min (the root) to the max, the left-most node
     * of the bottom row.
     */
    path = 0;
    for (k = 0, n = heap->heap_nelts; n >= 2; k += 1, n /= 2) {
        path = (path << 1) | (n & 1);
    }
    /* now traverse the heap using the path we calculated in the previous step.
     */
    max = &heap->heap_min;
    while (k > 0) {
        if (path & 1) {
            max = &(*max)->right;
        } else {
            max = &(*max)->left;
        }
        path >>= 1;
        k -= 1;
    }
    heap->heap_nelts -= 1;

    /* unlink the max node. */
    child = *max;
    *max = NULL;

    if (child == node) {
        /* we're removing either the max or the last node in the tree. */
        if (child == heap->heap_min) {
            heap->heap_min = NULL;
        }
        return;
    }
    /* replace the to be deleted node with the max node. */
    child->left = node->left;
    child->right = node->right;
    child->parent = node->parent;

    if (child->left != NULL) {
        child->left->parent = child;
    }
    if (child->right != NULL) {
        child->right->parent = child;
    }
    if (node->parent == NULL) {
        heap->heap_min = child;
    } else if (node->parent->left == node) {
        node->parent->left = child;
    } else {
        node->parent->right = child;
    }
    /* walk down the subtree and check at each node if the heap property holds.
     * it's a min heap so parent < child must be true.  if the parent is bigger,
     * swap it with the smallest child.
     */
    for (;;) {
        smallest = child;
        if (child->left != NULL && heap->compare(child->left, smallest)) {
            smallest = child->left;
        }
        if (child->right != NULL && heap->compare(child->right, smallest)) {
            smallest = child->right;
        }
        if (smallest == child) {
            break;
        }
        _heap_node_swap(heap, child, smallest);
    }
    /* walk up the subtree and check that each parent is less than the node
     * this is required, because `max` node is not guaranteed to be the
     * actual maximum in tree
     */
    while (child->parent != NULL && heap->compare(child, child->parent)) {
        _heap_node_swap(heap, child->parent, child);
    }
}

void xcomm_heap_dequeue(xcomm_heap_t* heap) {
    xcomm_heap_remove(heap, heap->heap_min);
}

bool xcomm_heap_empty(xcomm_heap_t* heap) {
    return heap->heap_nelts == 0 && heap->heap_min == NULL;
}

xcomm_heap_node_t* xcomm_heap_min(xcomm_heap_t* heap) {
    return heap->heap_min;
}