#include "avl.h"

#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

struct avl_node {
	void *value;
	void *data;
	struct avl_node *left;
	struct avl_node *right;
	int8_t balance;
};

struct avl_tree {
	struct avl_node *root;
	int (*cmp_func)(void *new_value, void *node_value);
};

void avl_tree_init(struct avl_tree *tree, int (*cmp_func)(void *new_value, void *node_value)) {
	assert(tree != NULL);
	assert(cmp_func != NULL);

	tree->root = NULL;
	tree->cmp_func = cmp_func;
}

void _rotate_left(struct avl_node **root) {
	// The prior_right node will become the new root
	struct avl_node *prior_right = (*root)->right;

	// Everything to the left of prior_right is attached to root's right
	(*root)->right = prior_right->left;

	// Calculate root's new balance assuming the tree has been regularly balanced
	if ((*root)->right != NULL) {
		(*root)->balance += LEFT;
	} else {
		(*root)->balance += 2 * LEFT;
	}

	// prior_right's left is now the old root
	prior_right->left = *root;
	prior_right->balance += LEFT;

	// prior_right is the new root
	*root = prior_right;
}

void _rotate_right(struct avl_node **root) {
	// The prior_left node will become the new root
	struct avl_node *prior_left = (*root)->left;

	// Everything to the right of prior_left is attached to root's left
	(*root)->left = prior_left->right;

	// Calculate root's new balance assuming the tree has been regularly balanced
	if ((*root)->left != NULL) {
		(*root)->balance += RIGHT;
	} else {
		(*root)->balance += 2 * RIGHT;
	}

	// prior_left's left is now the old root
	prior_left->left = *root;
	prior_left->balance += RIGHT;

	// prior_left is the new root
	*root = prior_left;
}

void _balance_left(struct avl_node **node) {
	if ((*node)->left->balance == RIGHT) {
		_rotate_left(&(*node)->left);
	}

	_rotate_right(node);
}

void _balance_right(struct avl_node **node) {
	if ((*node)->right->balance == LEFT) {
		_rotate_right(&(*node)->right);
	}

	_rotate_left(node);
}

int _add_helper(
		struct avl_node **node, void *value, int (*cmp_func)(void *new_value, void *node_value),
		void *data, bool *increase) {
	assert(node != NULL);

	if (*node == NULL) {
		*node = malloc(sizeof(**node));
		if (*node == NULL) {
			perror("malloc(sizeof(**root))");
			return -errno;
		}

		(*node)->value = value;
		(*node)->data = data;
		(*node)->left = NULL;
		(*node)->right = NULL;
		(*node)->balance = BALANCED;

		// The tree got longer here and may now be unbalanced
		*increase = true;

		return true;
	} else {
		int direction = cmp_func(value, (*node)->value);

		int did_add;
		if (direction < 0) {
			// Add on left
			did_add = _add_helper(&(*node)->left, value, cmp_func, data, increase);
			if (did_add < 0) {
				fprintf(stderr, "avl_tree_add_helper() error: %d\n", did_add);
				return did_add;
			}

			if (*increase) {
				// The left side got longer for this node
				(*node)->balance += LEFT;

				if ((*node)->balance == BALANCED) {
					// The overall height of the tree did not increase (the addition balanced this node)
					*increase = false;
				} else if ((*node)->balance < LEFT) {
					// The overall height of the tree increased out of bounds on the left side
					_balance_left(node);
					*increase = false;
				}
			}

			return did_add;
		} else if (0 < direction) {
			// Add on right
			did_add = _add_helper(&(*node)->right, value, cmp_func, data, increase);
			if (did_add < 0) {
				fprintf(stderr, "avl_tree_add_helper() error: %d\n", did_add);
				return did_add;
			}

			if (*increase) {
				// The right side got longer for this node
				(*node)->balance += RIGHT;

				if ((*node)->balance == BALANCED) {
					// The overall height of the tree did not increase (the addition balanced this node)
					*increase = false;
				} else if ((*node)->balance > RIGHT) {
					// The overall height of the tree increased out of bounds on the right side
					_balance_right(node);
					*increase = false;
				}
			}

			return did_add;
		} else {
			// There already exists a node with this value
			return false;
		}
	}
}

int avl_tree_add(struct avl_tree *tree, void *value, void *data) {
	bool increase = true;
	return _add_helper(&(tree->root), value, tree->cmp_func, data, &increase);
}
