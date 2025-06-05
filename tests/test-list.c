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

static void test_init_and_empty(void) {
    xcomm_list_t list;
    xcomm_list_init(&list);

    assert(xcomm_list_empty(&list));
    assert(xcomm_list_head(&list) == xcomm_list_sentinel(&list));
    assert(xcomm_list_tail(&list) == xcomm_list_sentinel(&list));
}

static void test_insert_head(void) {
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

static void test_insert_tail(void) {
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

static void test_remove_node(void) {
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

static void test_traversal_edge_cases(void) {
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

static void
verify_list_contents(xcomm_list_t* list, const int* expected, size_t count) {
    size_t             len = 0;
    xcomm_list_node_t* node = xcomm_list_head(list);
    while (node != xcomm_list_sentinel(list)) {
        len++;
        node = xcomm_list_next(node);
    }
    assert(len == count);

    if (count > 0) {
        node = xcomm_list_head(list);
        for (size_t i = 0; i < count; i++) {
            test_data_t* data = xcomm_list_data(node, test_data_t, node);
            assert(data->value == expected[i]);
            node = xcomm_list_next(node);
        }
    }
}

static void test_swap_empty_lists(void) {
    xcomm_list_t list1, list2;
    xcomm_list_init(&list1);
    xcomm_list_init(&list2);

    assert(xcomm_list_empty(&list1));
    assert(xcomm_list_empty(&list2));

    xcomm_list_swap(&list1, &list2);

    assert(xcomm_list_empty(&list1));
    assert(xcomm_list_empty(&list2));
}

static void test_swap_same_list(void) {
    xcomm_list_t list;
    xcomm_list_init(&list);

    test_data_t data = {.value = 42};
    xcomm_list_insert_head(&list, &data.node);

    assert(!xcomm_list_empty(&list));

    xcomm_list_swap(&list, &list);

    assert(!xcomm_list_empty(&list));
    xcomm_list_node_t* head = xcomm_list_head(&list);
    test_data_t*       first = xcomm_list_data(head, test_data_t, node);
    assert(first->value == 42);
}

static void test_swap_first_empty(void) {
    xcomm_list_t list1, list2;
    xcomm_list_init(&list1);
    xcomm_list_init(&list2);

    test_data_t data1 = {.value = 10};
    test_data_t data2 = {.value = 20};
    xcomm_list_insert_tail(&list2, &data1.node);
    xcomm_list_insert_tail(&list2, &data2.node);

    xcomm_list_swap(&list1, &list2);

    const int expected1[] = {10, 20};
    verify_list_contents(&list1, expected1, 2);

    assert(xcomm_list_empty(&list2));
}

static void test_swap_second_empty(void) {
    xcomm_list_t list1, list2;
    xcomm_list_init(&list1);
    xcomm_list_init(&list2);

    test_data_t data1 = {.value = 30};
    test_data_t data2 = {.value = 40};
    xcomm_list_insert_head(&list1, &data2.node);
    xcomm_list_insert_head(&list1, &data1.node);

    xcomm_list_swap(&list1, &list2);

    const int expected2[] = {30, 40};
    verify_list_contents(&list2, expected2, 2);

    assert(xcomm_list_empty(&list1));
}

static void test_swap_both_non_empty(void) {
    xcomm_list_t list1, list2;
    xcomm_list_init(&list1);
    xcomm_list_init(&list2);

    test_data_t data1 = {.value = 100};
    test_data_t data2 = {.value = 200};
    xcomm_list_insert_tail(&list1, &data1.node);
    xcomm_list_insert_tail(&list1, &data2.node);

    test_data_t data3 = {.value = 300};
    test_data_t data4 = {.value = 400};
    test_data_t data5 = {.value = 500};
    xcomm_list_insert_tail(&list2, &data3.node);
    xcomm_list_insert_tail(&list2, &data4.node);
    xcomm_list_insert_tail(&list2, &data5.node);

    xcomm_list_swap(&list1, &list2);

    const int expected1[] = {300, 400, 500};
    verify_list_contents(&list1, expected1, 3);

    const int expected2[] = {100, 200};
    verify_list_contents(&list2, expected2, 2);
}

static void test_swap_complex_case(void) {
    xcomm_list_t list1, list2;
    xcomm_list_init(&list1);
    xcomm_list_init(&list2);

    test_data_t items[6] = {
        {.value = 1},
        {.value = 2},
        {.value = 3},
        {.value = 4},
        {.value = 5},
        {.value = 6}};

    for (int i = 0; i < 3; i++) {
        xcomm_list_insert_tail(&list1, &items[i].node);
    }
    for (int i = 3; i < 6; i++) {
        xcomm_list_insert_tail(&list2, &items[i].node);
    }
    xcomm_list_swap(&list1, &list2);

    const int expected1[] = {4, 5, 6};
    verify_list_contents(&list1, expected1, 3);

    const int expected2[] = {1, 2, 3};
    verify_list_contents(&list2, expected2, 3);

    for (int i = 0; i < 6; i++) {
        assert(items[i].node.next != NULL);
        assert(items[i].node.prev != NULL);
        assert(items[i].node.next != &items[i].node);
        assert(items[i].node.prev != &items[i].node);
    }
}

int main(void) {
    test_init_and_empty();
    test_insert_head();
    test_insert_tail();
    test_remove_node();
    test_traversal_edge_cases();

    test_swap_empty_lists();
    test_swap_same_list();
    test_swap_first_empty();
    test_swap_second_empty();
    test_swap_both_non_empty();
    test_swap_complex_case();
    return 0;
}