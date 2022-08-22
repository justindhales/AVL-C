#ifndef AVL_C_SRC_AVL_H
#define AVL_C_SRC_AVL_H

#include <stdint.h>

struct avl_node;
struct avl_tree;

void avl_tree_init(struct avl_tree *tree, int (*cmp_func)(void *new_value, void *node_value));

int avl_tree_add(struct avl_tree *tree, void *new_value, void *new_data);

int avl_tree_get(struct avl_tree *tree, void *search_value, void **node_data);

int avl_tree_remove(struct avl_tree *tree, void *search_value, void **node_value, void **node_data);

#endif  // AVL_C_SRC_AVL_H
