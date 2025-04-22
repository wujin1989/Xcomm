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

#include "xcomm-rbtree.h"

#define RB_RED 0
#define RB_BLACK 1

static inline void
_rbtree_rotate_left(xcomm_rbtree_node_t* node, xcomm_rbtree_t* tree) {
    xcomm_rbtree_node_t* right = node->right;

    if ((node->right = right->left)) {
        right->left->parent = node;
    }
    right->left = node;
    if ((right->parent = node->parent)) {
        if (node == node->parent->left) {
            node->parent->left = right;
        } else {
            node->parent->right = right;
        }
    } else {
        tree->root = right;
    }
    node->parent = right;
}

static inline void
_rbtree_rotate_right(xcomm_rbtree_node_t* node, xcomm_rbtree_t* tree) {
    xcomm_rbtree_node_t* left = node->left;

    if ((node->left = left->right)) {
        left->right->parent = node;
    }
    left->right = node;
    if ((left->parent = node->parent)) {
        if (node == node->parent->right) {
            node->parent->right = left;
        } else {
            node->parent->left = left;
        }
    } else {
        tree->root = left;
    }
    node->parent = left;
}

static inline void _rbtree_link_node(
    xcomm_rbtree_node_t*  node,
    xcomm_rbtree_node_t*  parent,
    xcomm_rbtree_node_t** rb_link) {
    node->parent = parent;
    node->color = RB_RED;
    node->left = node->right = NULL;
    *rb_link = node;
}

static inline void
_rbtree_insert_color(xcomm_rbtree_t* tree, xcomm_rbtree_node_t* node) {
    xcomm_rbtree_node_t *parent, *gparent;

    while ((parent = node->parent) && parent->color == RB_RED) {
        gparent = parent->parent;
        if (parent == gparent->left) {
            {
                register xcomm_rbtree_node_t* uncle = gparent->right;
                if (uncle && uncle->color == RB_RED) {
                    uncle->color = RB_BLACK;
                    parent->color = RB_BLACK;
                    gparent->color = RB_RED;
                    node = gparent;
                    continue;
                }
            }
            if (parent->right == node) {
                register xcomm_rbtree_node_t* tmp;
                _rbtree_rotate_left(parent, tree);
                tmp = parent;
                parent = node;
                node = tmp;
            }
            parent->color = RB_BLACK;
            gparent->color = RB_RED;
            _rbtree_rotate_right(gparent, tree);
        } else {
            {
                register xcomm_rbtree_node_t* uncle = gparent->left;
                if (uncle && uncle->color == RB_RED) {
                    uncle->color = RB_BLACK;
                    parent->color = RB_BLACK;
                    gparent->color = RB_RED;
                    node = gparent;
                    continue;
                }
            }
            if (parent->left == node) {
                register xcomm_rbtree_node_t* tmp;
                _rbtree_rotate_right(parent, tree);
                tmp = parent;
                parent = node;
                node = tmp;
            }
            parent->color = RB_BLACK;
            gparent->color = RB_RED;
            _rbtree_rotate_left(gparent, tree);
        }
    }
    tree->root->color = RB_BLACK;
}

static inline void _rbtree_erase_color(
    xcomm_rbtree_node_t* node,
    xcomm_rbtree_node_t* parent,
    xcomm_rbtree_t*      tree) {
    xcomm_rbtree_node_t* other;
    while ((!node || node->color == RB_BLACK) && node != tree->root) {
        if (parent->left == node) {
            other = parent->right;
            if (other->color == RB_RED) {
                other->color = RB_BLACK;
                parent->color = RB_RED;
                _rbtree_rotate_left(parent, tree);
                other = parent->right;
            }
            if ((!other->left || other->left->color == RB_BLACK) &&
                (!other->right || other->right->color == RB_BLACK)) {
                other->color = RB_RED;
                node = parent;
                parent = node->parent;
            } else {
                if (!other->right || other->right->color == RB_BLACK) {
                    if (other->left) {
                        other->left->color = RB_BLACK;
                    }
                    other->color = RB_RED;
                    _rbtree_rotate_right(other, tree);
                    other = parent->right;
                }
                other->color = parent->color;
                parent->color = RB_BLACK;
                if (other->right) {
                    other->right->color = RB_BLACK;
                }
                _rbtree_rotate_left(parent, tree);
                node = tree->root;
                break;
            }
        } else {
            other = parent->left;
            if (other) {
                if (other->color == RB_RED) {
                    other->color = RB_BLACK;
                    parent->color = RB_RED;
                    _rbtree_rotate_right(parent, tree);
                    other = parent->left;
                }
                if ((!other->left || other->left->color == RB_BLACK) &&
                    (!other->right || other->right->color == RB_BLACK)) {
                    other->color = RB_RED;
                    node = parent;
                    parent = node->parent;
                } else {
                    if (!other->left || other->left->color == RB_BLACK) {
                        if (other->right) {
                            other->right->color = RB_BLACK;
                        }
                        other->color = RB_RED;
                        _rbtree_rotate_left(other, tree);
                        other = parent->left;
                    }
                    other->color = parent->color;
                    parent->color = RB_BLACK;
                    if (other->left) {
                        other->left->color = RB_BLACK;
                    }
                    _rbtree_rotate_right(parent, tree);
                    node = tree->root;
                    break;
                }
            }
        }
    }
    if (node) {
        node->color = RB_BLACK;
    }
}

int builtin_keycmp_i8(xcomm_rbtree_key_t* k1, xcomm_rbtree_key_t* k2) {
    if (k1->i8 < k2->i8) {
        return -1;
    }
    if (k1->i8 > k2->i8) {
        return 1;
    }
    return 0;
}

int builtin_keycmp_i16(xcomm_rbtree_key_t* k1, xcomm_rbtree_key_t* k2) {
    if (k1->i16 < k2->i16) {
        return -1;
    }
    if (k1->i16 > k2->i16) {
        return 1;
    }
    return 0;
}

int builtin_keycmp_i32(xcomm_rbtree_key_t* k1, xcomm_rbtree_key_t* k2) {
    if (k1->i32 < k2->i32) {
        return -1;
    }
    if (k1->i32 > k2->i32) {
        return 1;
    }
    return 0;
}

int builtin_keycmp_i64(xcomm_rbtree_key_t* k1, xcomm_rbtree_key_t* k2) {
    if (k1->i64 < k2->i64) {
        return -1;
    }
    if (k1->i64 > k2->i64) {
        return 1;
    }
    return 0;
}

int builtin_keycmp_u8(xcomm_rbtree_key_t* k1, xcomm_rbtree_key_t* k2) {
    if (k1->u8 < k2->u8) {
        return -1;
    }
    if (k1->u8 > k2->u8) {
        return 1;
    }
    return 0;
}

int builtin_keycmp_u16(xcomm_rbtree_key_t* k1, xcomm_rbtree_key_t* k2) {
    if (k1->u16 < k2->u16) {
        return -1;
    }
    if (k1->u16 > k2->u16) {
        return 1;
    }
    return 0;
}

int builtin_keycmp_u32(xcomm_rbtree_key_t* k1, xcomm_rbtree_key_t* k2) {
    if (k1->u32 < k2->u32) {
        return -1;
    }
    if (k1->u32 > k2->u32) {
        return 1;
    }
    return 0;
}

int builtin_keycmp_u64(xcomm_rbtree_key_t* k1, xcomm_rbtree_key_t* k2) {
    if (k1->u64 < k2->u64) {
        return -1;
    }
    if (k1->u64 > k2->u64) {
        return 1;
    }
    return 0;
}

int builtin_keycmp_str(xcomm_rbtree_key_t* k1, xcomm_rbtree_key_t* k2) {
    return strcmp(k1->str, k2->str);
}

void xcomm_rbtree_init(
    xcomm_rbtree_t* tree,
    int (*keycmp)(xcomm_rbtree_key_t*, xcomm_rbtree_key_t*)) {
    tree->root = NULL;
    tree->compare = keycmp;
}

void xcomm_rbtree_insert(xcomm_rbtree_t* tree, xcomm_rbtree_node_t* node) {
    xcomm_rbtree_node_t** p = &(tree->root);
    xcomm_rbtree_node_t*  parent = NULL;
    while (*p) {
        parent = *p;
        int r = tree->compare(&node->key, &parent->key);
        if (r < 0) {
            p = &(*p)->left;
        } else if (r > 0) {
            p = &(*p)->right;
        } else {
            return;
        }
    }
    _rbtree_link_node(node, parent, p);
    _rbtree_insert_color(tree, node);
}

void xcomm_rbtree_erase(xcomm_rbtree_t* tree, xcomm_rbtree_node_t* node) {
    xcomm_rbtree_node_t *child, *parent;
    int                  color;

    if (!node->left) {
        child = node->right;
    } else if (!node->right) {
        child = node->left;
    } else {
        xcomm_rbtree_node_t *old = node, *left;
        node = node->right;
        while ((left = node->left)) {
            node = left;
        }
        if (old->parent) {
            if (old->parent->left == old) {
                old->parent->left = node;
            } else {
                old->parent->right = node;
            }
        } else {
            tree->root = node;
        }
        child = node->right;
        parent = node->parent;
        color = node->color;
        if (parent == old) {
            parent = node;
        } else {
            if (child) {
                child->parent = parent;
            }
            parent->left = child;
            node->right = old->right;
            old->right->parent = node;
        }
        node->parent = old->parent;
        node->color = old->color;
        node->left = old->left;
        old->left->parent = node;

        goto color;
    }
    parent = node->parent;
    color = node->color;
    if (child) {
        child->parent = parent;
    }
    if (parent) {
        if (parent->left == node) {
            parent->left = child;
        } else {
            parent->right = child;
        }
    } else {
        tree->root = child;
    }
color:
    if (color == RB_BLACK) {
        _rbtree_erase_color(child, parent, tree);
    }
}

bool xcomm_rbtree_empty(xcomm_rbtree_t* tree) {
    return xcomm_rbtree_first(tree) == NULL;
}

xcomm_rbtree_node_t*
xcomm_rbtree_find(xcomm_rbtree_t* tree, xcomm_rbtree_key_t key) {
    xcomm_rbtree_node_t* n = tree->root;
    while (n) {
        int r = tree->compare(&key, &n->key);
        if (r < 0) {
            n = n->left;
        } else if (r > 0) {
            n = n->right;
        } else {
            return n;
        }
    }
    return NULL;
}

xcomm_rbtree_node_t* xcomm_rbtree_next(xcomm_rbtree_node_t* node) {
    xcomm_rbtree_node_t* parent;
    if (node->parent == node) {
        return NULL;
    }
    /* If we have a right-hand child, go down and then left as far
       as we can. */
    if (node->right) {
        node = node->right;
        while (node->left) {
            node = node->left;
        }
        return (xcomm_rbtree_node_t*)node;
    }
    /* No right-hand children.  Everything down and left is
       smaller than us, so any 'next' node must be in the general
       direction of our parent. Go up the tree; any time the
       ancestor is a right-hand child of its parent, keep going
       up. First time it's a left-hand child of its parent, said
       parent is our 'next' node. */
    while ((parent = node->parent) && node == parent->right) {
        node = parent;
    }
    return parent;
}

xcomm_rbtree_node_t* xcomm_rbtree_prev(xcomm_rbtree_node_t* node) {
    xcomm_rbtree_node_t* parent;
    if (node->parent == node) {
        return NULL;
    }
    /* If we have a left-hand child, go down and then right as far
       as we can. */
    if (node->left) {
        node = node->left;
        while (node->right) {
            node = node->right;
        }
        return (xcomm_rbtree_node_t*)node;
    }
    /* No left-hand children. Go up till we find an ancestor which
       is a right-hand child of its parent */
    while ((parent = node->parent) && node == parent->left) {
        node = parent;
    }
    return parent;
}

xcomm_rbtree_node_t* xcomm_rbtree_first(xcomm_rbtree_t* tree) {
    xcomm_rbtree_node_t* n;
    n = tree->root;
    if (!n) {
        return NULL;
    }
    while (n->left) {
        n = n->left;
    }
    return n;
}

xcomm_rbtree_node_t* xcomm_rbtree_last(xcomm_rbtree_t* tree) {
    xcomm_rbtree_node_t* n;
    n = tree->root;
    if (!n) {
        return NULL;
    }
    while (n->right) {
        n = n->right;
    }
    return n;
}