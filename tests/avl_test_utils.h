#ifndef AVL_C_TESTS_AVL_TEST_UTILS_H
#define AVL_C_TESTS_AVL_TEST_UTILS_H

#include <check.h>

#include "avl.h"

struct avl_node {
	void const *value;
	void const *data;
	struct avl_node *left;
	struct avl_node *right;
	int8_t balance;
};

enum weight { LEFT = -1, BALANCED = 0, RIGHT = 1 };

int int64_t_cmp(void const *new_value, void const *node_value);

struct avl_tree *new_tree();

void check_tree(struct avl_tree *tree);

void free_tree(struct avl_tree *tree);

int run(Suite *suite);

#endif  // AVL_C_TESTS_AVL_TEST_UTILS_H
