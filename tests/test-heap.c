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

#include "xcomm-heap.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

typedef struct test_data_s {
    int               value;
    xcomm_heap_node_t node;
} test_data_t;

static int test_min_compare(xcomm_heap_node_t* a, xcomm_heap_node_t* b) {
    test_data_t* data_a = xcomm_heap_data(a, test_data_t, node);
    test_data_t* data_b = xcomm_heap_data(b, test_data_t, node);
    return data_a->value < data_b->value;
}

static void test_heap_init() {
    xcomm_heap_t heap;
    xcomm_heap_init(&heap, test_min_compare);

    assert(xcomm_heap_empty(&heap));
    assert(heap.heap_nelts == 0);
    assert(heap.heap_min == NULL);
}

static void test_single_element() {
    xcomm_heap_t heap;
    xcomm_heap_init(&heap, test_min_compare);

    test_data_t data = {.value = 42};
    xcomm_heap_insert(&heap, &data.node);

    assert(!xcomm_heap_empty(&heap));
    assert(heap.heap_nelts == 1);
    assert(xcomm_heap_min(&heap) == &data.node);

    xcomm_heap_remove(&heap, &data.node);
    assert(xcomm_heap_empty(&heap));
    assert(heap.heap_nelts == 0);
}

static void test_heap_property() {
    xcomm_heap_t heap;
    xcomm_heap_init(&heap, test_min_compare);

    test_data_t data[] = {
        {.value = 5}, {.value = 3}, {.value = 7}, {.value = 1}};

    for (int i = 0; i < sizeof(data) / sizeof(data[0]); i++) {
        xcomm_heap_insert(&heap, &data[i].node);
    }
    assert(heap.heap_nelts == 4);
    test_data_t* min =
        (test_data_t*)xcomm_heap_data(xcomm_heap_min(&heap), test_data_t, node);
    assert(min->value == 1);

    xcomm_heap_dequeue(&heap);
    min =
        (test_data_t*)xcomm_heap_data(xcomm_heap_min(&heap), test_data_t, node);
    assert(min->value == 3);
    assert(heap.heap_nelts == 3);
}

static void test_remove_middle() {
    xcomm_heap_t heap;
    xcomm_heap_init(&heap, test_min_compare);

    test_data_t data[] = {
        {.value = 2}, {.value = 6}, {.value = 4}, {.value = 8}};

    for (int i = 0; i < sizeof(data) / sizeof(data[0]); i++) {
        xcomm_heap_insert(&heap, &data[i].node);
    }
    xcomm_heap_remove(&heap, &data[2].node);
    assert(heap.heap_nelts == 3);

    test_data_t* min =
        (test_data_t*)xcomm_heap_data(xcomm_heap_min(&heap), test_data_t, node);
    assert(min->value == 2);
}

static void test_edge_cases() {
    xcomm_heap_t heap;
    xcomm_heap_init(&heap, test_min_compare);

    xcomm_heap_dequeue(&heap);

    test_data_t dup_data[] = {{.value = 5}, {.value = 5}};
    xcomm_heap_insert(&heap, &dup_data[0].node);
    xcomm_heap_insert(&heap, &dup_data[1].node);
    assert(heap.heap_nelts == 2);
}

int main(void) {
    test_heap_init();
    test_single_element();
    test_heap_property();
    test_remove_middle();
    test_edge_cases();
    return 0;
}