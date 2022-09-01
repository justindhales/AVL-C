#include "avl_test_utils.h"

#include <stdlib.h>

int int64_t_cmp(void const *new_value, void const *node_value) {
	// Yes, we are casting to an int64_t. This is for testing.
	return (int64_t)new_value - (int64_t)node_value;
}

struct avl_tree *new_tree() {
	struct avl_tree *tree = malloc(sizeof(struct avl_tree));
	avl_tree_init(tree, int64_t_cmp);

	return tree;
}

int _check_node(struct avl_node const *node, void *previous_value) {
	// Make sure the ordering is correct

	// The 64 bit value is stored inside the void * in the node, but we're passing around a pointer to
	// a malloc()'d int64_t for comparison.
	ck_assert(*(int64_t *)previous_value < (int64_t)node->value);
	*(int64_t *)previous_value = (int64_t)node->value;

	// Make sure the balance is within bounds
	ck_assert(LEFT <= node->balance);
	ck_assert(node->balance <= RIGHT);

	return 0;
}

void check_tree(struct avl_tree *tree) {
	size_t previous_value = INT64_MIN;

	avl_tree_traverse(tree, NULL, NULL, _check_node, &previous_value, NULL, NULL);
}

int _free_node(struct avl_node const *node, void *arg __attribute__((unused))) {
	free((struct avl_node *)node);
	return 0;
}

void free_tree(struct avl_tree *tree) {
	avl_tree_traverse(tree, NULL, NULL, NULL, NULL, _free_node, NULL);
	free(tree);
}

int run(Suite *suite) {
	int number_failed;
	SRunner *runner;

	runner = srunner_create(suite);

	srunner_run_all(runner, CK_VERBOSE);
	number_failed = srunner_ntests_failed(runner);
	srunner_free(runner);
	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
