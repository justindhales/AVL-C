#ifndef AVL_C_SRC_AVL_H
#define AVL_C_SRC_AVL_H

#include <stdint.h>

struct avl_node;
struct avl_tree;

void const *avl_node_value(struct avl_node const *node);
void const *avl_node_data(struct avl_node const *node);

void avl_tree_init(
		struct avl_tree *tree, int (*cmp_func)(void const *new_value, void const *node_value));

int avl_tree_add(struct avl_tree *tree, void const *new_value, void const *new_data);

int avl_tree_get(struct avl_tree const *tree, void const *search_value, void const **node_data);

int avl_tree_remove(
		struct avl_tree *tree, void const *search_value, void const **node_value,
		void const **node_data);

int avl_tree_traverse(
		struct avl_tree const *tree, int (*preorder_func)(struct avl_node const *node, void *arg),
		int (*inorder_func)(struct avl_node const *node, void *arg),
		int (*postorder_func)(struct avl_node const *node, void *arg), void *arg);

#endif  // AVL_C_SRC_AVL_H
