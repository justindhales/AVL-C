#include "avl.h"

#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

struct avl_node {
	void const *value;
	void const *data;
	struct avl_node *left;
	struct avl_node *right;
	int8_t balance;
};

struct avl_tree {
	struct avl_node *root;
	int (*cmp_func)(void const *new_value, void const *node_value);
};

enum weight { LEFT = -1, BALANCED = 0, RIGHT = 1 };

void const *avl_node_value(struct avl_node const *node) {
	assert(node != NULL);
	return node->value;
}

void const *avl_node_data(struct avl_node const *node) {
	assert(node != NULL);
	return node->data;
}

void avl_tree_init(
		struct avl_tree *tree, int (*cmp_func)(void const *new_value, void const *node_value)) {
	assert(tree != NULL);
	assert(cmp_func != NULL);

	tree->root = NULL;
	tree->cmp_func = cmp_func;
}

void _rotate_left(struct avl_node **root) {
	assert(root != NULL);
	assert(*root != NULL);
	assert((*root)->right != NULL);
	// Should rotate left only when the root node is balanced toward the right
	assert((*root)->balance >= RIGHT);

	// The prior_right node will eventually be saved in root's current position
	struct avl_node *prior_right = (*root)->right;

	// Everything to the left of prior_right is attached to root's right
	(*root)->right = prior_right->left;

	// Calculate root's new balance assuming the tree has been regularly balanced
	(*root)->balance = (LEFT * ((*root)->left != NULL)) + (RIGHT * ((*root)->right != NULL));

	// prior_right's left is now the old root
	prior_right->left = *root;
	prior_right->balance += LEFT;

	// prior_right is the new root
	*root = prior_right;
}

void _rotate_right(struct avl_node **root) {
	assert(root != NULL);
	assert(*root != NULL);
	assert((*root)->left != NULL);
	// Should rotate right only when the root node is balanced toward the left
	assert((*root)->balance <= LEFT);

	// The prior_left node will eventually be saved in root's current position
	struct avl_node *prior_left = (*root)->left;

	// Everything to the right of prior_left is attached to root's left
	(*root)->left = prior_left->right;

	// Calculate root's new balance assuming the tree has been regularly balanced
	(*root)->balance = (LEFT * ((*root)->left != NULL)) + (RIGHT * ((*root)->right != NULL));

	// prior_left's left is now the old root
	prior_left->left = *root;
	prior_left->balance += RIGHT;

	// prior_left is the new root
	*root = prior_left;
}

void _balance_left(struct avl_node **node, bool *decrease) {
	assert(node != NULL);
	assert(*node != NULL);
	// assert(decrease != NULL);
	assert((*node)->left != NULL);

	if ((*node)->left->balance == RIGHT) {
		_rotate_left(&(*node)->left);
	} else if ((*node)->left->balance == BALANCED) {
		// The height of the subtree will remain the same after rotating. Occurs only during removals.
		*decrease = false;
	}

	_rotate_right(node);
}

void _balance_right(struct avl_node **node, bool *decrease) {
	assert(node != NULL);
	assert(*node != NULL);
	// assert(decrease != NULL);
	assert((*node)->right != NULL);

	if ((*node)->right->balance == LEFT) {
		_rotate_right(&(*node)->right);
	} else if ((*node)->right->balance == BALANCED) {
		// The height of the subtree will remain the same after rotating. Occurs only during removals.
		*decrease = false;
	}

	_rotate_left(node);
}

int _add_helper(
		struct avl_node **root, void const *value, void const *data,
		int (*cmp_func)(void const *new_value, void const *node_value), bool *increase) {
	assert(root != NULL);
	assert(value != NULL);
	assert(data != NULL);
	assert(cmp_func != NULL);
	assert(increase != NULL);

	if (*root == NULL) {
		*root = malloc(sizeof(**root));
		if (*root == NULL) {
			perror("malloc(sizeof(**root))");
			return -errno;
		}

		(*root)->value = value;
		(*root)->data = data;
		(*root)->left = NULL;
		(*root)->right = NULL;
		(*root)->balance = BALANCED;

		// The tree got longer here and may now be unbalanced
		*increase = true;

		return true;
	} else {
		int direction = cmp_func(value, (*root)->value);

		int did_add;
		if (direction < 0) {
			// Add on left
			did_add = _add_helper(&(*root)->left, value, data, cmp_func, increase);
			if (did_add < 0) {
				fprintf(stderr, "avl_tree_add_helper() error: %d\n", did_add);
				return did_add;
			}

			if (*increase) {
				// The left side got longer for this node
				(*root)->balance += LEFT;

				// The overall height of the tree did not increase (the addition balanced this node)
				if ((*root)->balance == BALANCED) {
					*increase = false;
				}
				// The overall height of the tree increased out of bounds on the left side
				else if ((*root)->balance < LEFT) {
					_balance_left(root, NULL);
					*increase = false;
				}
			}

			return did_add;
		} else if (0 < direction) {
			// Add on right
			did_add = _add_helper(&(*root)->right, value, data, cmp_func, increase);
			if (did_add < 0) {
				fprintf(stderr, "avl_tree_add_helper() error: %d\n", did_add);
				return did_add;
			}

			if (*increase) {
				// The right side got longer for this node
				(*root)->balance += RIGHT;

				// The overall height of the tree did not increase (the addition balanced this node)
				if ((*root)->balance == BALANCED) {
					*increase = false;
				}
				// The overall height of the tree increased out of bounds on the right side
				else if ((*root)->balance > RIGHT) {
					_balance_right(root, NULL);
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

int avl_tree_add(struct avl_tree *tree, void const *new_value, void const *new_data) {
	assert(tree != NULL);
	assert(new_value != NULL);
	assert(new_data != NULL);

	bool increase = true;
	return _add_helper(&(tree->root), new_value, new_data, tree->cmp_func, &increase);
}

int _get_helper(
		struct avl_node *node, void const *value, void const **data,
		int (*cmp_func)(void const *new_value, void const *node_value)) {
	assert(value != NULL);
	assert(data != NULL);
	// assert(*data == NULL);
	assert(cmp_func != NULL);

	if (node == NULL) {
		// We didn't find the node
		return false;
	} else {
		int direction = cmp_func(value, node->value);

		if (direction < 0) {
			// Search left
			return _get_helper(node->left, value, data, cmp_func);
		} else if (0 < direction) {
			// Search right
			return _get_helper(node->right, value, data, cmp_func);
		} else {
			// We've found it
			*data = node->data;
			return true;
		}
	}
}

int avl_tree_get(struct avl_tree const *tree, void const *search_value, void const **node_data) {
	assert(tree != NULL);
	assert(search_value != NULL);
	assert(node_data != NULL);

	return _get_helper((tree)->root, search_value, node_data, tree->cmp_func);
}

int _remove_helper(
		struct avl_node **root, void const *search_value, void const **node_value,
		void const **node_data, int (*cmp_func)(void const *new_value, void const *node_value),
		bool *decrease) {
	assert(root != NULL);
	assert(search_value != NULL);
	assert(node_value != NULL);
	assert(node_data != NULL);
	assert(cmp_func != NULL);
	assert(decrease != NULL);

	if (*root == NULL) {
		// We didn't find the node
		return false;
	} else {
		int direction = cmp_func(search_value, (*root)->value);

		int did_remove;

		if (direction < 0) {
			// Remove from left side
			did_remove =
					_remove_helper(&(*root)->left, search_value, node_value, node_data, cmp_func, decrease);

			if (*decrease) {
				// Since a node was removed on the left, the right side is now longer
				(*root)->balance += RIGHT;

				// If the overall height of this node did not decrease
				if ((*root)->balance == RIGHT) {
					*decrease = false;
				}
				// If the overall height of this node decreased out of bounds on the right side
				else if (RIGHT < (*root)->balance) {
					_balance_left(root, decrease);
				}
			}

			return did_remove;
		} else if (0 < direction) {
			// Remove from right side
			did_remove =
					_remove_helper(&(*root)->right, search_value, node_value, node_data, cmp_func, decrease);

			if (*decrease) {
				// Since a node was removed on the right, the left side is now longer
				(*root)->balance += LEFT;

				// If the overall height of this node did not decrease
				if ((*root)->balance == LEFT) {
					*decrease = false;
				}
				// If the overall height of this node decreased out of bounds on the left side
				else if ((*root)->balance < LEFT) {
					_balance_right(root, decrease);
				}
			}

			return did_remove;
		} else {
			// This is the node to remove

			struct avl_node *old_node = *root;

			// Save this node's value and data just in case it needs to be freed externally
			*node_value = (*root)->value;
			*node_data = (*root)->data;

			// If the root has no left child or no children
			if ((*root)->left == NULL) {
				// Put root's right node in root's position
				*root = (*root)->right;
				free(old_node);
				*decrease = true;
				return true;
			}
			// If the root has no right child
			else if ((*root)->right == NULL) {
				// Put root's left child in root's position
				*root = (*root)->left;
				free(old_node);
				*decrease = true;
				return true;
			}
			// If the root has two children
			else {
				// Find the inorder predecessor node (this will be a leaf node)
				struct avl_node *predecessor;
				for (predecessor = (*root)->left; predecessor->right != NULL;
						 predecessor = predecessor->right) {}

				void const *predecessor_value;
				void const *predecessor_data;
				// Now remove the inorder predecessor from the tree
				did_remove = _remove_helper(
						&(*root)->left, predecessor->value, &predecessor_value, &predecessor_data, cmp_func,
						decrease);

				// Copy the value and data previously in the inorder predecessor into root
				(*root)->value = predecessor_value;
				(*root)->data = predecessor_data;

				// Because the predecessor node was on the left, the right side of the tree is possibly
				// longer depending on whether prior rebalancings fixed things
				if (*decrease) {
					// Tree got taller on right side because a node was removed from the left side
					(*root)->balance += RIGHT;

					// If the overall height of this node did not increase
					if ((*root)->balance == RIGHT) {
						*decrease = false;
					}
					// If the overall height of the tree increased out of bounds on the right side
					else if (RIGHT < (*root)->balance) {
						_balance_right(root, decrease);
					}
					// else: this node is now balanced, which means that the opposite side of the parent node
					// is now relatively taller. Don't change the decrease variable because the next node
					// might need to be rebalanced.
				}

				return did_remove;
			}
		}
	}
}

int avl_tree_remove(
		struct avl_tree *tree, void const *search_value, void const **node_value,
		void const **node_data) {
	assert(tree != NULL);
	assert(search_value != NULL);
	assert(node_value != NULL);
	assert(node_data != NULL);

	bool decrease = false;
	return _remove_helper(
			&tree->root, search_value, node_value, node_data, tree->cmp_func, &decrease);
}

int _traverse_helper(
		struct avl_node *root, int (*preorder_func)(struct avl_node const *node, void *arg),
		int (*inorder_func)(struct avl_node const *node, void *arg),
		int (*postorder_func)(struct avl_node const *node, void *arg), void *arg) {
	if (root == NULL) {
		return 0;
	}

	int rc;

	// Apply pre-order function on node
	if (preorder_func != NULL) {
		rc = preorder_func(root, arg);
		if (rc <= -1) {
			return rc;
		}
	}

	// Go down the left branch
	rc = _traverse_helper(root->left, preorder_func, inorder_func, postorder_func, arg);
	if (rc <= -1) {
		return rc;
	}

	// Apply in-order function on node
	if (inorder_func != NULL) {
		rc = inorder_func(root, arg);
		if (rc <= -1) {
			return rc;
		}
	}

	// Go down the right branch
	rc = _traverse_helper(root->right, preorder_func, inorder_func, postorder_func, arg);
	if (rc <= -1) {
		return rc;
	}

	// Apply post-order function on node
	if (postorder_func != NULL) {
		rc = postorder_func(root, arg);
		if (rc <= -1) {
			return rc;
		}
	}

	return 0;
}

int avl_tree_traverse(
		struct avl_tree const *tree, int (*preorder_func)(struct avl_node const *node, void *arg),
		int (*inorder_func)(struct avl_node const *node, void *arg),
		int (*postorder_func)(struct avl_node const *node, void *arg), void *arg) {
	assert(tree != NULL);
	return _traverse_helper(tree->root, preorder_func, inorder_func, postorder_func, arg);
}
