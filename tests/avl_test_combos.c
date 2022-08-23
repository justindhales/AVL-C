#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#include "avl_test_utils.h"

START_TEST(test_balance_right_right) {
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
	struct avl_tree *tree = new_tree();

	int rc;

	rc = avl_tree_add(tree, (void *)0, (void *)1);
	ck_assert(rc == true);

	rc = avl_tree_add(tree, (void *)4, (void *)5);
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

START_TEST(test_balance_right_right_right) {
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

START_TEST(test_balance_random) {
	srand(time(NULL));

	// Create 1000 trees
	for (int iterations = 0; iterations < 1000; ++iterations) {
		struct avl_tree *tree = new_tree();

		// Of 1000 nodes (more or less) each
		for (int num_node = 0; num_node < 1000; ++num_node) {
			avl_tree_add(tree, (void *)(int64_t)rand(), (void *)(int64_t)rand());
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

	tcase_add_test(tcase, test_balance_random);

	suite_add_tcase(suite, tcase);

	return suite;
}

int main() {
	return run(test_suite());
}
