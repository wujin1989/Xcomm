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
#include "xcomm-list.h"

typedef struct test_data_s {
    int               value;
    xcomm_list_node_t node;
} test_data_t;

void test_init_and_empty(void) {
    xcomm_list_t list;
    xcomm_list_init(&list);

    assert(xcomm_list_empty(&list));
    assert(xcomm_list_head(&list) == xcomm_list_sentinel(&list));
    assert(xcomm_list_tail(&list) == xcomm_list_sentinel(&list));
}

void test_insert_head(void) {
    xcomm_list_t list;
    xcomm_list_init(&list);

    test_data_t data1 = {.value = 1};
    test_data_t data2 = {.value = 2};

    xcomm_list_insert_head(&list, &data1.node);
    xcomm_list_insert_head(&list, &data2.node);
    assert(!xcomm_list_empty(&list));

    xcomm_list_node_t* head = xcomm_list_head(&list);
    test_data_t*       first = xcomm_list_data(head, test_data_t, node);
    assert(first->value == 2);

    xcomm_list_node_t* next = xcomm_list_next(head);
    test_data_t*       second = xcomm_list_data(next, test_data_t, node);
    assert(second->value == 1);

    xcomm_list_node_t* tail = xcomm_list_tail(&list);
    assert(tail == &data1.node);
}

void test_insert_tail(void) {
    xcomm_list_t list;
    xcomm_list_init(&list);

    test_data_t data1 = {.value = 1};
    test_data_t data2 = {.value = 2};

    xcomm_list_insert_tail(&list, &data1.node);
    xcomm_list_insert_tail(&list, &data2.node);

    assert(!xcomm_list_empty(&list));

    xcomm_list_node_t* tail = xcomm_list_tail(&list);
    test_data_t*       last = xcomm_list_data(tail, test_data_t, node);
    assert(last->value == 2);

    xcomm_list_node_t* prev = xcomm_list_prev(tail);
    test_data_t*       first = xcomm_list_data(prev, test_data_t, node);
    assert(first->value == 1);
}

void test_remove_node(void) {
    xcomm_list_t list;
    xcomm_list_init(&list);

    test_data_t data1 = {.value = 1};
    test_data_t data2 = {.value = 2};

    xcomm_list_insert_head(&list, &data1.node);
    xcomm_list_insert_head(&list, &data2.node);

    xcomm_list_remove(&data2.node);
    assert(xcomm_list_head(&list) == &data1.node);

    xcomm_list_remove(&data1.node);
    assert(xcomm_list_empty(&list));
}

void test_traversal_edge_cases(void) {
    xcomm_list_t list;
    xcomm_list_init(&list);

    assert(
        xcomm_list_next(xcomm_list_sentinel(&list)) ==
        xcomm_list_sentinel(&list));
    assert(
        xcomm_list_prev(xcomm_list_sentinel(&list)) ==
        xcomm_list_sentinel(&list));

    test_data_t data = {.value = 42};
    xcomm_list_insert_head(&list, &data.node);
    assert(xcomm_list_next(&data.node) == xcomm_list_sentinel(&list));
    assert(xcomm_list_prev(&data.node) == xcomm_list_sentinel(&list));
}

int main(void) {
    test_init_and_empty();
    test_insert_head();
    test_insert_tail();
    test_remove_node();
    test_traversal_edge_cases();
    return 0;
}