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
#include <stdlib.h>
#include <string.h>

#include "xcomm-rbtree.h"
#include "xcomm-utils.h"

#define RB_RED 0
#define RB_BLACK 1

typedef struct test_data_s {
    int                 value;
    xcomm_rbtree_node_t node;
} test_data_t;

static int check_rbtree_properties(
    xcomm_rbtree_node_t* node, xcomm_rbtree_node_t* parent) {
    if (node == NULL) {
        return 1;
    }
    if (node->color == RB_RED) {
        assert(parent == NULL || parent->color == RB_BLACK);
        assert(node->left == NULL || node->left->color == RB_BLACK);
        assert(node->right == NULL || node->right->color == RB_BLACK);
    }
    int left_black = check_rbtree_properties(node->left, node);
    int right_black = check_rbtree_properties(node->right, node);

    assert(left_black == right_black);
    return left_black + (node->color == RB_BLACK ? 1 : 0);
}

static void validate_rbtree(xcomm_rbtree_t* tree) {
    if (tree->root) {
        assert(tree->root->color == RB_BLACK);
    }
    check_rbtree_properties(tree->root, NULL);
}

void test_int32_keys(void) {
    xcomm_rbtree_t tree;
    xcomm_rbtree_init(&tree, xcomm_rbtree_keycmp_i32);

    int         values[] = {5, 2, 8, 1, 4, 7, 9, 3, 6};
    test_data_t nodes[sizeof(values) / sizeof(values[0])];

    for (int i = 0; i < sizeof(values) / sizeof(values[0]); ++i) {
        nodes[i].value = values[i];
        nodes[i].node.key.i32 = values[i];
        xcomm_rbtree_insert(&tree, &nodes[i].node);
        validate_rbtree(&tree);
    }
    for (int i = 0; i < sizeof(values) / sizeof(values[0]); ++i) {
        xcomm_rbtree_key_t   key = {.i32 = values[i]};
        xcomm_rbtree_node_t* found = xcomm_rbtree_find(&tree, key);
        assert(found != NULL);
        assert(xcomm_rbtree_data(found, test_data_t, node)->value == values[i]);
    }
    int                  prev = -1;
    xcomm_rbtree_node_t* n = xcomm_rbtree_first(&tree);
    while (n) {
        int current = xcomm_rbtree_data(n, test_data_t, node)->value;
        assert(current > prev);
        prev = current;
        n = xcomm_rbtree_next(n);
    }
    assert(prev == 9);

    xcomm_rbtree_erase(&tree, &nodes[3].node);
    assert(xcomm_rbtree_find(&tree, (xcomm_rbtree_key_t){.i32 = 1}) == NULL);
    validate_rbtree(&tree);
}

void test_string_keys(void) {
    xcomm_rbtree_t tree;
    xcomm_rbtree_init(&tree, xcomm_rbtree_keycmp_str);

    const char* cities[] = {"Beijing", "Shanghai", "Guangzhou", "Shenzhen"};
    test_data_t nodes[4];

    for (int i = 0; i < 4; ++i) {
        nodes[i].node.key.str = strdup(cities[i]);
        xcomm_rbtree_insert(&tree, &nodes[i].node);
        validate_rbtree(&tree);
    }
    xcomm_rbtree_key_t   key = {.str = "Shanghai"};
    xcomm_rbtree_node_t* found = xcomm_rbtree_find(&tree, key);
    assert(found != NULL);
    assert(strcmp(found->key.str, "Shanghai") == 0);

    for (int i = 0; i < 4; ++i) {
        free(nodes[i].node.key.str);
    }
}

void test_edge_cases(void) {
    xcomm_rbtree_t tree;
    xcomm_rbtree_init(&tree, xcomm_rbtree_keycmp_i32);

    assert(xcomm_rbtree_empty(&tree));
    assert(xcomm_rbtree_first(&tree) == NULL);
    assert(xcomm_rbtree_last(&tree) == NULL);

    test_data_t single = {.value = 100};
    single.node.key.i32 = 100;
    xcomm_rbtree_insert(&tree, &single.node);
    validate_rbtree(&tree);
    assert(xcomm_rbtree_first(&tree) == &single.node);
    assert(xcomm_rbtree_last(&tree) == &single.node);

    xcomm_rbtree_erase(&tree, &single.node);
    assert(xcomm_rbtree_empty(&tree));
}

void test_different_int_types(void) {
    {
        xcomm_rbtree_t tree;
        xcomm_rbtree_init(&tree, xcomm_rbtree_keycmp_u64);

        test_data_t data = {.value = 1};
        data.node.key.u64 = 0xFFFFFFFFFFFFFFFF;
        xcomm_rbtree_insert(&tree, &data.node);

        xcomm_rbtree_key_t key = {.u64 = 0xFFFFFFFFFFFFFFFF};
        assert(xcomm_rbtree_find(&tree, key) == &data.node);
    }
    {
        xcomm_rbtree_t tree;
        xcomm_rbtree_init(&tree, xcomm_rbtree_keycmp_i8);

        test_data_t min = {.node.key.i8 = -128};
        test_data_t max = {.node.key.i8 = 127};
        xcomm_rbtree_insert(&tree, &min.node);
        xcomm_rbtree_insert(&tree, &max.node);

        assert(xcomm_rbtree_first(&tree)->key.i8 == -128);
        assert(xcomm_rbtree_last(&tree)->key.i8 == 127);
    }
}

void test_delete_and_reinsert(void) {
    xcomm_rbtree_t tree;
    xcomm_rbtree_init(&tree, xcomm_rbtree_keycmp_i32);

    test_data_t data[5];
    int         values[] = {5, 3, 7, 2, 8};

    for (int i = 0; i < 5; ++i) {
        data[i].node.key.i32 = values[i];
        xcomm_rbtree_insert(&tree, &data[i].node);
    }
    xcomm_rbtree_erase(&tree, &data[1].node);
    assert(xcomm_rbtree_find(&tree, (xcomm_rbtree_key_t){.i32 = 3}) == NULL);
    validate_rbtree(&tree);

    xcomm_rbtree_insert(&tree, &data[1].node);
    assert(xcomm_rbtree_find(&tree, (xcomm_rbtree_key_t){.i32 = 3}) != NULL);
    validate_rbtree(&tree);
}

void test_duplicate_keys(void) {
    {
        xcomm_rbtree_t tree;
        xcomm_rbtree_init(&tree, xcomm_rbtree_keycmp_i32);

        test_data_t nodes[5];
        for (int i = 0; i < 5; ++i) {
            nodes[i].node.key.i32 = 42;
            xcomm_rbtree_insert(&tree, &nodes[i].node);
        }
        xcomm_rbtree_node_t* found =
            xcomm_rbtree_find(&tree, (xcomm_rbtree_key_t){.i32 = 42});
        assert(found == &nodes[0].node);

        xcomm_rbtree_node_t* n = xcomm_rbtree_next(found);
        assert(n == NULL);
    }
    {
        xcomm_rbtree_t tree;
        xcomm_rbtree_init(&tree, xcomm_rbtree_keycmp_str);

        test_data_t nodes[3];
        const char* values[] = {"dup", "dup", "dup"};

        for (int i = 0; i < 3; ++i) {
            nodes[i].node.key.str = strdup(values[i]);
            xcomm_rbtree_insert(&tree, &nodes[i].node);
        }
        xcomm_rbtree_node_t* found =
            xcomm_rbtree_find(&tree, (xcomm_rbtree_key_t){.str = "dup"});
        assert(found == &nodes[0].node);

        for (int i = 0; i < 3; ++i) {
            free(nodes[i].node.key.str);
        }
    }
}

int main(void) {
    test_int32_keys();
    test_string_keys();
    test_edge_cases();
    test_different_int_types();
    test_delete_and_reinsert();
    test_duplicate_keys();
    return 0;
}