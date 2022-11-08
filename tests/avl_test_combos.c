#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "avl_test_utils.h"

#define NUM_VALUES 100000

/**
 * @brief Checks if a tree containing (0,1), (2,3) and (4,5) is balanced
 *
 * @param tree
 */
void check_three_tree(struct avl_node *root, int64_t offset) {
	/*
	   2+o
	  /   \
	0+o   4+o
	*/

	ck_assert(root != NULL);
	ck_assert(root->value == (void *)(2 + offset));
	ck_assert(root->data == (void *)(3 + offset));
	ck_assert(root->left != NULL);
	ck_assert(root->right != NULL);
	ck_assert(root->balance == BALANCED);

	ck_assert(root->left->value == (void *)(0 + offset));
	ck_assert(root->left->data == (void *)(1 + offset));
	ck_assert(root->left->left == NULL);
	ck_assert(root->left->right == NULL);
	ck_assert(root->left->balance == BALANCED);

	ck_assert(root->right->value == (void *)(4 + offset));
	ck_assert(root->right->data == (void *)(5 + offset));
	ck_assert(root->right->left == NULL);
	ck_assert(root->right->right == NULL);
	ck_assert(root->right->balance == BALANCED);
}

#define test_add(tree, v) avl_tree_add(tree, (void *)v, (void *)(v + 1))

void _test_remove(struct avl_tree *tree, void *v, bool success) {
	void const *node_value = NULL;
	void const *node_data = NULL;
	ck_assert(avl_tree_remove(tree, v, &node_value, &node_data) == success);
	if (success) {
		ck_assert(node_value == v);
		ck_assert(node_data == (void *)((int8_t *)v + 1));
	} else {
		ck_assert(node_value == NULL);
		ck_assert(node_data == NULL);
	}
}

#define test_remove(tree, v, success) _test_remove(tree, (void *)v, success)

START_TEST(test_balance_right_right) {
	printf("test balance right-right\n");
	struct avl_tree *tree = create_tree();

	/*
	0
	 \
	  2
	   \
	    4
	*/

	for (int64_t i = 0; i <= 4; i += 2) {
		ck_assert(test_add(tree, i) == true);
	}

	/*
	  2
	 / \
	0   4
	*/

	check_three_tree(tree->root, 0);

	free_tree(tree);
}

END_TEST

START_TEST(test_balance_full_right_right) {
	printf("test balance full right-right\n");
	struct avl_tree *tree = create_tree();

	/*
	  2
	 / \
	0   6
	   / \
	  4   8
	       \
	        10
	*/

	ck_assert(test_add(tree, 2) == true);
	ck_assert(test_add(tree, 0) == true);
	ck_assert(test_add(tree, 6) == true);
	ck_assert(test_add(tree, 4) == true);
	ck_assert(test_add(tree, 8) == true);
	ck_assert(test_add(tree, 10) == true);

	/*
	    6
	   / \
	  2   8
	 / \   \
	0   4   10
	*/

	struct avl_node *root = tree->root;
	ck_assert(root != NULL);
	ck_assert(root->value == (void *)6);
	ck_assert(root->data == (void *)7);
	ck_assert(root->left != NULL);
	ck_assert(root->right != NULL);
	ck_assert(root->balance == BALANCED);

	check_three_tree(root->left, 0);

	ck_assert(root->right->value == (void *)8);
	ck_assert(root->right->data == (void *)9);
	ck_assert(root->right->left == NULL);
	ck_assert(root->right->right != NULL);
	ck_assert(root->right->balance == RIGHT);

	ck_assert(root->right->right->value == (void *)10);
	ck_assert(root->right->right->data == (void *)11);
	ck_assert(root->right->right->left == NULL);
	ck_assert(root->right->right->right == NULL);
	ck_assert(root->right->right->balance == BALANCED);

	free_tree(tree);
}

END_TEST

START_TEST(test_balance_right_left) {
	printf("test balance right-left\n");
	struct avl_tree *tree = create_tree();

	/*
	0
	  \
	    4
	   /
	  2
	*/

	ck_assert(test_add(tree, 0) == true);
	ck_assert(test_add(tree, 4) == true);
	ck_assert(test_add(tree, 2) == true);

	/*
	  2
	 / \
	0   4
	*/

	check_three_tree(tree->root, 0);

	free_tree(tree);
}

START_TEST(test_balance_full_right_left) {
	printf("test balance full right-left\n");
	struct avl_tree *tree = create_tree();

	/*
	  2
	 /  \
	0     8
	     / \
	    6	  10
	   /
	  4
	*/

	ck_assert(test_add(tree, 2) == true);
	ck_assert(test_add(tree, 0) == true);
	ck_assert(test_add(tree, 8) == true);
	ck_assert(test_add(tree, 10) == true);
	ck_assert(test_add(tree, 6) == true);
	ck_assert(test_add(tree, 4) == true);

	/*
	    6
	   / \
	  2   8
	 / \   \
	0   4   10
	*/

	struct avl_node *root = tree->root;
	ck_assert(root != NULL);
	ck_assert(root->value == (void *)6);
	ck_assert(root->data == (void *)7);
	ck_assert(root->left != NULL);
	ck_assert(root->right != NULL);
	ck_assert(root->balance == BALANCED);

	check_three_tree(root->left, 0);

	ck_assert(root->right->value == (void *)8);
	ck_assert(root->right->data == (void *)9);
	ck_assert(root->right->left == NULL);
	ck_assert(root->right->right != NULL);
	ck_assert(root->right->balance == RIGHT);

	ck_assert(root->right->right->value == (void *)10);
	ck_assert(root->right->right->data == (void *)11);
	ck_assert(root->right->right->left == NULL);
	ck_assert(root->right->right->right == NULL);
	ck_assert(root->right->right->balance == BALANCED);

	free_tree(tree);
}

END_TEST

START_TEST(test_balance_right_right_right) {
	printf("test balance right-right-right\n");
	struct avl_tree *tree = create_tree();

	/*
	0
	 \
	  2
	   \
	    4
	     \
	      6
	       \
	        8
	*/

	for (int64_t i = 0; i <= 8; i += 2) {
		ck_assert(test_add(tree, i) == true);
		// Tree will be rebalanced after three additions
	}

	/*
	  2
	 /  \
	0     6
	     / \
	    4   8
	*/

	struct avl_node *root = tree->root;
	ck_assert(root != NULL);
	ck_assert(root->value == (void *)2);
	ck_assert(root->data == (void *)3);
	ck_assert(root->left != NULL);
	ck_assert(root->right != NULL);
	ck_assert(root->balance == RIGHT);

	ck_assert(root->left->value == (void *)0);
	ck_assert(root->left->data == (void *)1);
	ck_assert(root->left->left == NULL);
	ck_assert(root->left->right == NULL);
	ck_assert(root->left->balance == BALANCED);

	check_three_tree(root->right, 4);

	free_tree(tree);
}

END_TEST

START_TEST(test_balance_left_left) {
	printf("test balance left-left\n");
	struct avl_tree *tree = create_tree();

	/*
	    4
	   /
	  2
	 /
	0
	*/

	for (int64_t i = 4; i >= 0; i -= 2) {
		ck_assert(test_add(tree, i) == true);
	}

	/*
	  2
	 / \
	0   4
	*/

	check_three_tree(tree->root, 0);

	free_tree(tree);
}

END_TEST

START_TEST(test_balance_full_left_left) {
	printf("test balance full left-left\n");
	struct avl_tree *tree = create_tree();

	/*
	      8
	     / \
	    4   10
	   / \
	  2   6
	 /
	0
	*/

	ck_assert(test_add(tree, 8) == true);
	ck_assert(test_add(tree, 10) == true);
	ck_assert(test_add(tree, 4) == true);
	ck_assert(test_add(tree, 6) == true);
	ck_assert(test_add(tree, 2) == true);
	ck_assert(test_add(tree, 0) == true);

	/*
	    4
	   / \
	  2   8
	 /   / \
	0   6   10
	*/

	struct avl_node *root = tree->root;
	ck_assert(root != NULL);
	ck_assert(root->value == (void *)4);
	ck_assert(root->data == (void *)5);
	ck_assert(root->left != NULL);
	ck_assert(root->right != NULL);
	ck_assert(root->balance == BALANCED);

	check_three_tree(root->right, 6);

	ck_assert(root->left->value == (void *)2);
	ck_assert(root->left->data == (void *)3);
	ck_assert(root->left->left != NULL);
	ck_assert(root->left->right == NULL);
	ck_assert(root->left->balance == LEFT);

	ck_assert(root->left->left->value == (void *)0);
	ck_assert(root->left->left->data == (void *)1);
	ck_assert(root->left->left->left == NULL);
	ck_assert(root->left->left->right == NULL);
	ck_assert(root->left->left->balance == BALANCED);

	free_tree(tree);
}

END_TEST

START_TEST(test_balance_left_right) {
	printf("test balance left-right\n");
	struct avl_tree *tree = create_tree();

	/*
	    4
	  /
	0
	 \
	  2
	*/

	ck_assert(test_add(tree, 4) == true);
	ck_assert(test_add(tree, 0) == true);
	ck_assert(test_add(tree, 2) == true);

	check_three_tree(tree->root, 0);

	free_tree(tree);
}

END_TEST

START_TEST(test_balance_full_left_right) {
	printf("test balance full left-right\n");
	struct avl_tree *tree = create_tree();

	/*
	      8
	    /  \
	  2     10
	 / \
	0   4
	     \
	      6
	*/

	ck_assert(test_add(tree, 8) == true);
	ck_assert(test_add(tree, 10) == true);
	ck_assert(test_add(tree, 2) == true);
	ck_assert(test_add(tree, 0) == true);
	ck_assert(test_add(tree, 4) == true);
	ck_assert(test_add(tree, 6) == true);

	/*
	    4
	   / \
	  2   8
	 /   / \
	0   6   10
	*/

	struct avl_node *root = tree->root;
	ck_assert(root != NULL);
	ck_assert(root->value == (void *)4);
	ck_assert(root->data == (void *)5);
	ck_assert(root->left != NULL);
	ck_assert(root->right != NULL);
	ck_assert(root->balance == BALANCED);

	check_three_tree(root->right, 6);

	ck_assert(root->left->value == (void *)2);
	ck_assert(root->left->data == (void *)3);
	ck_assert(root->left->left != NULL);
	ck_assert(root->left->right == NULL);
	ck_assert(root->left->balance == LEFT);

	ck_assert(root->left->left->value == (void *)0);
	ck_assert(root->left->left->data == (void *)1);
	ck_assert(root->left->left->left == NULL);
	ck_assert(root->left->left->right == NULL);
	ck_assert(root->left->left->balance == BALANCED);

	free_tree(tree);
}

END_TEST

START_TEST(test_balance_left_left_left) {
	printf("test balance left-left-left\n");
	struct avl_tree *tree = create_tree();

	/*
	        8
	       /
	      6
	     /
	    4
	   /
	  2
	 /
	0
	*/

	for (int64_t i = 8; i >= 0; i -= 2) {
		ck_assert(test_add(tree, i) == true);
		// Tree will be rebalanced after three additions
	}

	/*
	      6
	    /  \
	  2     8
	 / \
	0   4
	*/

	struct avl_node *root = tree->root;
	ck_assert(root != NULL);
	ck_assert(root->value == (void *)6);
	ck_assert(root->data == (void *)7);
	ck_assert(root->left != NULL);
	ck_assert(root->right != NULL);
	ck_assert(root->balance == LEFT);

	ck_assert(root->right->value == (void *)8);
	ck_assert(root->right->data == (void *)9);
	ck_assert(root->right->left == NULL);
	ck_assert(root->right->right == NULL);
	ck_assert(root->right->balance == BALANCED);

	check_three_tree(root->left, 0);

	free_tree(tree);
}

END_TEST

START_TEST(test_add_remove_all) {
	printf("test add remove all\n");

	// int rc;

	struct avl_tree *tree;

	int64_t values[NUM_VALUES];
	int64_t v;
	int v_i;

	int i;

	// Create 10 trees
	for (i = 0; i < 10; ++i) {
		tree = create_tree();

		// Add NUM_VALUES random values

		for (v_i = 0; v_i < NUM_VALUES; ++v_i) {
			v = (int64_t)rand() % NUM_VALUES;
			avl_tree_add(tree, (void *)v, (void *)(v + 1));
			values[v_i] = v;
		}

		check_tree(tree);

		void const *node_value;
		void const *node_data;
		// Remove those same NUM_VALUES values
		for (v_i = 0; v_i < NUM_VALUES; ++v_i) {
			avl_tree_remove(tree, (void *)values[v_i], &node_value, &node_data);
		}

		ck_assert(tree->root == NULL);

		free_tree(tree);
	}
}

END_TEST

START_TEST(test_balance_random) {
	printf("test balance random\n");

	// Create 10 trees
	for (int iteration = 0; iteration < 10; ++iteration) {
		struct avl_tree *tree = create_tree();

		for (int64_t num_node = 0; num_node < NUM_VALUES; ++num_node) {
			// Of completely (well, mostly) random values
			avl_tree_add(tree, (void *)(int64_t)rand(), (void *)(int64_t)rand());
		}

		check_tree(tree);

		// Now randomly remove some nodes
		void const *value;
		void const *data;
		for (int64_t num_node = 0; num_node < NUM_VALUES; ++num_node) {
			avl_tree_remove(tree, (void *)(int64_t)rand(), &value, &data);
		}

		check_tree(tree);

		free_tree(tree);
	}
}

END_TEST

Suite *test_suite() {
	Suite *suite = suite_create("test_suite");

	TCase *tcase = tcase_create("case");

	tcase_add_test(tcase, test_balance_right_right);
	tcase_add_test(tcase, test_balance_full_right_right);

	tcase_add_test(tcase, test_balance_right_left);
	tcase_add_test(tcase, test_balance_full_right_left);

	tcase_add_test(tcase, test_balance_right_right_right);

	tcase_add_test(tcase, test_balance_left_left);
	tcase_add_test(tcase, test_balance_full_left_left);

	tcase_add_test(tcase, test_balance_left_right);
	tcase_add_test(tcase, test_balance_full_left_right);

	tcase_add_test(tcase, test_balance_left_left_left);

	tcase_add_test(tcase, test_add_remove_all);

	tcase_add_test(tcase, test_balance_random);

	suite_add_tcase(suite, tcase);

	return suite;
}

int main() {
	srand(time(NULL));
	return run(test_suite());
}
