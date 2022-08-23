#include <stdbool.h>
#include <stdlib.h>

#include "avl_test_utils.h"

START_TEST(test_init) {
	struct avl_tree *tree = malloc(sizeof(struct avl_tree));
	avl_tree_init(tree, int64_t_cmp);

	ck_assert(tree->root == NULL);
	ck_assert(tree->cmp_func == int64_t_cmp);

	free_tree(tree);
}

END_TEST

START_TEST(test_add) {
	struct avl_tree *tree = new_tree();

	int rc;

	rc = avl_tree_add(tree, (void *)0, (void *)1);
	ck_assert(rc == true);

	struct avl_node *root = tree->root;

	ck_assert(root != NULL);
	ck_assert(root->value == (void *)0);
	ck_assert(root->data == (void *)1);
	ck_assert(root->left == NULL);
	ck_assert(root->right == NULL);
	ck_assert(root->balance == BALANCED);

	free_tree(tree);
}

END_TEST

START_TEST(test_add_double) {
	struct avl_tree *tree = new_tree();

	int rc;

	rc = avl_tree_add(tree, (void *)0, (void *)1);
	ck_assert(rc == true);

	rc = avl_tree_add(tree, (void *)0, (void *)2);
	ck_assert(rc == false);

	struct avl_node *root = tree->root;
	ck_assert(root != NULL);
	ck_assert(root->value == (void *)0);
	ck_assert(root->data == (void *)1);
	ck_assert(root->left == NULL);
	ck_assert(root->right == NULL);
	ck_assert(root->balance == BALANCED);

	free_tree(tree);
}

START_TEST(test_add_right) {
	struct avl_tree *tree = new_tree();

	int rc;

	rc = avl_tree_add(tree, (void *)0, (void *)1);
	ck_assert(rc == true);

	rc = avl_tree_add(tree, (void *)2, (void *)3);
	ck_assert(rc == true);

	struct avl_node *root = tree->root;
	ck_assert(root != NULL);
	ck_assert(root->value == (void *)0);
	ck_assert(root->data == (void *)1);
	ck_assert(root->left == NULL);
	ck_assert(root->right != NULL);
	ck_assert(root->balance == RIGHT);

	ck_assert(root->right->value == (void *)2);
	ck_assert(root->right->data == (void *)3);
	ck_assert(root->right->left == NULL);
	ck_assert(root->right->right == NULL);
	ck_assert(root->right->balance == BALANCED);

	free_tree(tree);
}

END_TEST

START_TEST(test_add_left) {
	struct avl_tree *tree = new_tree();

	int rc;

	rc = avl_tree_add(tree, (void *)2, (void *)3);
	ck_assert(rc == true);

	rc = avl_tree_add(tree, (void *)0, (void *)1);
	ck_assert(rc == true);

	struct avl_node *root = tree->root;
	ck_assert(root != NULL);
	ck_assert(root->value == (void *)2);
	ck_assert(root->data == (void *)3);
	ck_assert(root->left != NULL);
	ck_assert(root->right == NULL);
	ck_assert(root->balance == LEFT);

	ck_assert(root->left->value == (void *)0);
	ck_assert(root->left->data == (void *)1);
	ck_assert(root->left->left == NULL);
	ck_assert(root->left->right == NULL);
	ck_assert(root->left->balance == BALANCED);

	free_tree(tree);
}

END_TEST

START_TEST(test_remove) {
	struct avl_tree *tree = new_tree();

	int rc;

	rc = avl_tree_add(tree, (void *)0, (void *)1);
	ck_assert(rc == true);

	void const *removed_value;
	void const *removed_data;
	rc = avl_tree_remove(tree, (void *)0, &removed_value, &removed_data);
	ck_assert(rc == true);

	ck_assert(tree->root == NULL);
	ck_assert(removed_value == (void *)0);
	ck_assert(removed_data == (void *)1);

	free_tree(tree);
}

END_TEST

START_TEST(test_remove_double) {
	struct avl_tree *tree = new_tree();

	int rc;

	rc = avl_tree_add(tree, (void *)0, (void *)1);
	ck_assert(rc == true);

	void const *removed_value;
	void const *removed_data;
	rc = avl_tree_remove(tree, (void *)0, &removed_value, &removed_data);
	ck_assert(rc == true);

	ck_assert(tree->root == NULL);
	ck_assert(removed_value == (void *)0);
	ck_assert(removed_data == (void *)1);

	removed_value = NULL;
	removed_data = NULL;
	rc = avl_tree_remove(tree, (void *)1, &removed_value, &removed_data);
	ck_assert(rc == false);

	ck_assert(tree->root == NULL);
	ck_assert(removed_value == NULL);
	ck_assert(removed_data == NULL);

	free_tree(tree);
}

END_TEST

START_TEST(test_get) {
	struct avl_tree *tree = new_tree();

	int rc;

	rc = avl_tree_add(tree, (void *)0, (void *)1);
	ck_assert(rc == true);

	void const *node_data;
	rc = avl_tree_get(tree, (void *)0, &node_data);
	ck_assert(rc == true);

	ck_assert(tree->root != NULL);
	ck_assert(node_data == (void *)1);

	free_tree(tree);
}

END_TEST

START_TEST(test_get_fail) {
	struct avl_tree *tree = new_tree();

	int rc;

	void const *node_data = NULL;
	rc = avl_tree_get(tree, (void *)1, &node_data);
	ck_assert(rc == false);

	ck_assert(node_data == NULL);

	free_tree(tree);
}

END_TEST

Suite *test_suite() {
	Suite *suite = suite_create("test_suite");

	TCase *tcase = tcase_create("case");

	tcase_add_test(tcase, test_init);
	tcase_add_test(tcase, test_add);
	tcase_add_test(tcase, test_add_double);
	tcase_add_test(tcase, test_add_right);
	tcase_add_test(tcase, test_add_left);
	tcase_add_test(tcase, test_remove);
	tcase_add_test(tcase, test_remove_double);
	tcase_add_test(tcase, test_get);
	tcase_add_test(tcase, test_get_fail);

	// tcase_add_test(tcase, test_balance_right_right);
	// tcase_add_test(tcase, test_balance_right_left);

	suite_add_tcase(suite, tcase);

	return suite;
}

int main() {
	return run(test_suite());
}
