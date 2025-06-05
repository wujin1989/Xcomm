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

#include "xcomm-list.h"

void xcomm_list_init(xcomm_list_t* list) {
    (list)->next = (list);
    (list)->prev = (list);
}

void xcomm_list_insert_head(xcomm_list_t* list, xcomm_list_node_t* node) {
    /* init node */
    (node)->next = (node);
    (node)->prev = (node);

    /* insert node */
    (node)->next = (list)->next;
    (node)->next->prev = (node);
    (node)->prev = (list);
    (list)->next = (node);
}

void xcomm_list_insert_tail(xcomm_list_t* list, xcomm_list_node_t* node) {
    (node)->next = (node);
    (node)->prev = (node);

    (node)->prev = (list)->prev;
    (node)->prev->next = (node);
    (node)->next = (list);
    (list)->prev = (node);
}

void xcomm_list_remove(xcomm_list_node_t* node) {
    (node)->next->prev = (node)->prev;
    (node)->prev->next = (node)->next;
}

bool xcomm_list_empty(xcomm_list_t* list) {
    return (list) == (list)->prev;
}

xcomm_list_node_t* xcomm_list_head(xcomm_list_t* list) {
    return (list)->next;
}

xcomm_list_node_t* xcomm_list_tail(xcomm_list_t* list) {
    return (list)->prev;
}

xcomm_list_node_t* xcomm_list_next(xcomm_list_node_t* node) {
    return (node)->next;
}

xcomm_list_node_t* xcomm_list_prev(xcomm_list_node_t* node) {
    return (node)->prev;
}

xcomm_list_node_t* xcomm_list_sentinel(xcomm_list_t* list) {
    return (list);
}

void xcomm_list_swap(xcomm_list_t* list1, xcomm_list_t* list2) {
    if (list1 == list2) {
        return;
    }
    bool list1_empty = (list1->next == list1);
    bool list2_empty = (list2->next == list2);

    if (list1_empty && list2_empty) {
        return;
    }
    if (list1_empty) {
        list1->next = list2->next;
        list1->prev = list2->prev;
        list1->next->prev = list1;
        list1->prev->next = list1;

        list2->next = list2;
        list2->prev = list2;
        return;
    }

    if (list2_empty) {
        list2->next = list1->next;
        list2->prev = list1->prev;
        list2->next->prev = list2;
        list2->prev->next = list2;

        list1->next = list1;
        list1->prev = list1;
        return;
    }
    xcomm_list_node_t* temp_next = list1->next;
    xcomm_list_node_t* temp_prev = list1->prev;

    list1->next = list2->next;
    list1->prev = list2->prev;
    list2->next = temp_next;
    list2->prev = temp_prev;

    list1->next->prev = list1;
    list1->prev->next = list1;
    list2->next->prev = list2;
    list2->prev->next = list2;
}
