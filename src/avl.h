#ifndef SRC_AVL_H
#define SRC_AVL_H

#include <stdint.h>

struct avl_node;
struct avl_tree;

enum weight { LEFT = -1, BALANCED = 0, RIGHT = 1 };

void avl_tree_init(struct avl_tree *tree, int (*cmp_func)(void *new_value, void *node_value));

int avl_tree_add(struct avl_tree *tree, void *value, void *data);

int avl_tree_get(struct avl_tree *tree, void *value, void **data);

int avl_tree_remove(struct avl_tree *tree, void *value, void **data);

#endif  // SRC_AVL_H
