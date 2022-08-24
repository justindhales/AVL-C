#ifndef AVL_C_SRC_AVL_H
#define AVL_C_SRC_AVL_H

#include <stdint.h>

struct avl_node;

struct avl_tree {
	struct avl_node *root;
	int (*cmp_func)(void const *new_value, void const *node_value);
};

void const *avl_node_value(struct avl_node const *node);
void const *avl_node_data(struct avl_node const *node);

void avl_tree_init(
		struct avl_tree *tree, int (*cmp_func)(void const *new_value, void const *node_value));

int avl_tree_add(struct avl_tree *tree, void const *new_value, void const *new_data);

int avl_tree_get(struct avl_tree const *tree, void const *search_value, void const **node_data);

int avl_tree_remove(
		struct avl_tree *tree, void const *search_value, void const **node_value,
		void const **node_data);

int avl_subtree_traverse(
		struct avl_node const *root, int (*preorder_func)(struct avl_node const *node, void *arg),
		void *preorder_arg, int (*inorder_func)(struct avl_node const *node, void *arg),
		void *inorder_arg, int (*postorder_func)(struct avl_node const *node, void *arg),
		void *postorder_arg);

int avl_tree_traverse(
		struct avl_tree const *tree, int (*preorder_func)(struct avl_node const *node, void *arg),
		void *preorder_arg, int (*inorder_func)(struct avl_node const *node, void *arg),
		void *inorder_arg, int (*postorder_func)(struct avl_node const *node, void *arg),
		void *postorder_arg);

void avl_node_print(struct avl_node const *node);

int avl_subtree_print(struct avl_node const *root);

int avl_tree_print(struct avl_tree const *tree);

#endif  // AVL_C_SRC_AVL_H
