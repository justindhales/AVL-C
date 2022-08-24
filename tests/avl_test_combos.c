#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "avl_test_utils.h"

#define NUM_VALUES 100000

START_TEST(test_balance_right_right) {
	printf("test balance right-right\n");
	struct avl_tree *tree = new_tree();

	int rc;

	rc = avl_tree_add(tree, (void *)0, (void *)1);
	ck_assert(rc == true);

	rc = avl_tree_add(tree, (void *)2, (void *)3);
	ck_assert(rc == true);

	rc = avl_tree_add(tree, (void *)4, (void *)5);
	ck_assert(rc == true);

	struct avl_node *root = tree->root;
	ck_assert(root != NULL);
	ck_assert(root->value == (void *)2);
	ck_assert(root->data == (void *)3);
	ck_assert(root->left != NULL);
	ck_assert(root->right != NULL);
	ck_assert(root->balance == BALANCED);

	ck_assert(root->left->value == (void *)0);
	ck_assert(root->left->data == (void *)1);
	ck_assert(root->left->left == NULL);
	ck_assert(root->left->right == NULL);
	ck_assert(root->left->balance == BALANCED);

	ck_assert(root->right->value == (void *)4);
	ck_assert(root->right->data == (void *)5);
	ck_assert(root->right->left == NULL);
	ck_assert(root->right->right == NULL);
	ck_assert(root->right->balance == BALANCED);

	free_tree(tree);
}

END_TEST

START_TEST(test_balance_right_left) {
	printf("test balance right-left\n");
	struct avl_tree *tree = new_tree();

	int rc;

	rc = avl_tree_add(tree, (void *)0, (void *)1);
	ck_assert(rc == true);

	// avl_tree_print(tree);

	rc = avl_tree_add(tree, (void *)4, (void *)5);
	ck_assert(rc == true);

	// avl_tree_print(tree);

	rc = avl_tree_add(tree, (void *)2, (void *)3);
	ck_assert(rc == true);

	// avl_tree_print(tree);

	struct avl_node *root = tree->root;
	ck_assert(root != NULL);
	ck_assert(root->value == (void *)2);
	ck_assert(root->data == (void *)3);
	ck_assert(root->left != NULL);
	ck_assert(root->right != NULL);
	ck_assert(root->balance == BALANCED);

	ck_assert(root->left->value == (void *)0);
	ck_assert(root->left->data == (void *)1);
	ck_assert(root->left->left == NULL);
	ck_assert(root->left->right == NULL);
	ck_assert(root->left->balance == BALANCED);

	ck_assert(root->right->value == (void *)4);
	ck_assert(root->right->data == (void *)5);
	ck_assert(root->right->left == NULL);
	ck_assert(root->right->right == NULL);
	ck_assert(root->right->balance == BALANCED);

	free_tree(tree);
}

END_TEST

START_TEST(test_balance_right_right_right) {
	printf("test balance right-right-right\n");
	struct avl_tree *tree = new_tree();

	int rc;

	rc = avl_tree_add(tree, (void *)0, (void *)1);
	ck_assert(rc == true);

	rc = avl_tree_add(tree, (void *)2, (void *)3);
	ck_assert(rc == true);

	rc = avl_tree_add(tree, (void *)4, (void *)5);
	ck_assert(rc == true);

	struct avl_node *root = tree->root;
	ck_assert(root != NULL);
	ck_assert(root->value == (void *)2);
	ck_assert(root->data == (void *)3);
	ck_assert(root->left != NULL);
	ck_assert(root->right != NULL);
	ck_assert(root->balance == BALANCED);

	ck_assert(root->left->value == (void *)0);
	ck_assert(root->left->data == (void *)1);
	ck_assert(root->left->left == NULL);
	ck_assert(root->left->right == NULL);
	ck_assert(root->left->balance == BALANCED);

	ck_assert(root->right->value == (void *)4);
	ck_assert(root->right->data == (void *)5);
	ck_assert(root->right->left == NULL);
	ck_assert(root->right->right == NULL);
	ck_assert(root->right->balance == BALANCED);

	rc = avl_tree_add(tree, (void *)6, (void *)7);
	ck_assert(rc == true);

	ck_assert(root->balance == RIGHT);
	ck_assert(root->right->balance == RIGHT);
	ck_assert(root->right->right->balance == BALANCED);

	rc = avl_tree_add(tree, (void *)8, (void *)9);
	ck_assert(rc == true);

	ck_assert(root->balance == RIGHT);
	ck_assert(root->right->balance == BALANCED);
	ck_assert(root->right->left->balance == BALANCED);
	ck_assert(root->right->right->balance == BALANCED);

	check_tree(tree);

	free_tree(tree);
}

END_TEST

START_TEST(test_balance_left_left) {
	printf("test balance left-left\n");
	struct avl_tree *tree = new_tree();

	int rc;

	rc = avl_tree_add(tree, (void *)4, (void *)5);
	ck_assert(rc == true);

	rc = avl_tree_add(tree, (void *)2, (void *)3);
	ck_assert(rc == true);

	rc = avl_tree_add(tree, (void *)0, (void *)1);
	ck_assert(rc == true);

	struct avl_node *root = tree->root;
	ck_assert(root != NULL);
	ck_assert(root->value == (void *)2);
	ck_assert(root->data == (void *)3);
	ck_assert(root->left != NULL);
	ck_assert(root->right != NULL);
	ck_assert(root->balance == BALANCED);

	ck_assert(root->left->value == (void *)0);
	ck_assert(root->left->data == (void *)1);
	ck_assert(root->left->left == NULL);
	ck_assert(root->left->right == NULL);
	ck_assert(root->left->balance == BALANCED);

	ck_assert(root->right->value == (void *)4);
	ck_assert(root->right->data == (void *)5);
	ck_assert(root->right->left == NULL);
	ck_assert(root->right->right == NULL);
	ck_assert(root->right->balance == BALANCED);

	free_tree(tree);
}

END_TEST

START_TEST(test_balance_left_right) {
	printf("test balance left-right\n");
	struct avl_tree *tree = new_tree();

	int rc;

	rc = avl_tree_add(tree, (void *)4, (void *)5);
	ck_assert(rc == true);

	rc = avl_tree_add(tree, (void *)0, (void *)1);
	ck_assert(rc == true);

	rc = avl_tree_add(tree, (void *)2, (void *)3);
	ck_assert(rc == true);

	struct avl_node *root = tree->root;
	ck_assert(root != NULL);
	ck_assert(root->value == (void *)2);
	ck_assert(root->data == (void *)3);
	ck_assert(root->left != NULL);
	ck_assert(root->right != NULL);
	ck_assert(root->balance == BALANCED);

	ck_assert(root->left->value == (void *)0);
	ck_assert(root->left->data == (void *)1);
	ck_assert(root->left->left == NULL);
	ck_assert(root->left->right == NULL);
	ck_assert(root->left->balance == BALANCED);

	ck_assert(root->right->value == (void *)4);
	ck_assert(root->right->data == (void *)5);
	ck_assert(root->right->left == NULL);
	ck_assert(root->right->right == NULL);
	ck_assert(root->right->balance == BALANCED);

	free_tree(tree);
}

END_TEST

START_TEST(test_balance_left_left_left) {
	printf("test balance left-left-left\n");
	struct avl_tree *tree = new_tree();

	int rc;

	rc = avl_tree_add(tree, (void *)4, (void *)5);
	ck_assert(rc == true);

	rc = avl_tree_add(tree, (void *)2, (void *)3);
	ck_assert(rc == true);

	rc = avl_tree_add(tree, (void *)0, (void *)1);
	ck_assert(rc == true);

	struct avl_node *root = tree->root;
	ck_assert(root != NULL);
	ck_assert(root->value == (void *)2);
	ck_assert(root->data == (void *)3);
	ck_assert(root->left != NULL);
	ck_assert(root->right != NULL);
	ck_assert(root->balance == BALANCED);

	ck_assert(root->left->value == (void *)0);
	ck_assert(root->left->data == (void *)1);
	ck_assert(root->left->left == NULL);
	ck_assert(root->left->right == NULL);
	ck_assert(root->left->balance == BALANCED);

	ck_assert(root->right->value == (void *)4);
	ck_assert(root->right->data == (void *)5);
	ck_assert(root->right->left == NULL);
	ck_assert(root->right->right == NULL);
	ck_assert(root->right->balance == BALANCED);

	rc = avl_tree_add(tree, (void *)-2, (void *)-1);
	ck_assert(rc == true);

	ck_assert(root->balance == LEFT);
	ck_assert(root->left->balance == LEFT);
	ck_assert(root->left->left->balance == BALANCED);

	rc = avl_tree_add(tree, (void *)-4, (void *)-3);
	ck_assert(rc == true);

	ck_assert(root->balance == LEFT);
	ck_assert(root->left->balance == BALANCED);
	ck_assert(root->left->left->balance == BALANCED);
	ck_assert(root->left->right->balance == BALANCED);

	check_tree(tree);

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
		tree = new_tree();

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
		struct avl_tree *tree = new_tree();

		for (int64_t num_node = 0; num_node < NUM_VALUES; ++num_node) {
			// Of completely (well, mostly) random values
			avl_tree_add(tree, (void *)(int64_t)rand(), (void *)(int64_t)rand());
		}

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
	tcase_add_test(tcase, test_balance_right_left);
	tcase_add_test(tcase, test_balance_right_right_right);

	tcase_add_test(tcase, test_balance_left_left);
	tcase_add_test(tcase, test_balance_left_right);
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
