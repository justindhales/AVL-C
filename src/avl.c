#include "avl.h"

#include <assert.h>
#include <errno.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

enum weight { LEFT = -1, BALANCED = 0, RIGHT = 1 };

struct avl_node {
	void const *value;
	void const *data;
	struct avl_node *left;
	struct avl_node *right;
	int32_t balance;
};

int avl_tree_create(
    struct avl_tree **tree, int (*cmp_func)(void const *new_value, void const *node_value)) {
	assert(tree != NULL);
	assert(*tree == NULL);

	int rc;

	*tree = malloc(sizeof(**tree));
	if (*tree == NULL) {
		perror("malloc(sizeof(**tree))");
		rc = -errno;
		goto finish;
	}

	(*tree)->root = NULL;
	(*tree)->cmp_func = cmp_func;
	(*tree)->lock = malloc(sizeof(*(*tree)->lock));
	if ((*tree)->lock == NULL) {
		rc = -errno;
		goto finish;
	}
	rc = sem_init((*tree)->lock, false, 1);
	if (rc < 0) {
		rc = -errno;
		goto finish;
	}

finish:
	if (rc < 0 && *tree != NULL) {
		avl_tree_free(tree, NULL, NULL);
	}

	return rc;
}

void avl_tree_free(
    struct avl_tree **tree, int (*free_node_func)(struct avl_node const *node, void *arg),
    void *free_arg) {
	assert(tree != NULL);
	assert(*tree != NULL);

	// Free each node of the tree
	int rc = avl_tree_traverse(*tree, NULL, NULL, NULL, NULL, free_node_func, free_arg);
	assert(rc == 0);

	// Destroy the semaphore lock and free the associated memory
	if ((*tree)->lock != NULL) {
		sem_destroy((*tree)->lock);
		free((*tree)->lock);
	}

	free(*tree);
	*tree = NULL;
}

void const *avl_node_value(struct avl_node const *node) {
	assert(node != NULL);
	return node->value;
}

void const *avl_node_data(struct avl_node const *node) {
	assert(node != NULL);
	return node->data;
}

void _rotate_left(struct avl_node **root) {
	assert(root != NULL);
	assert(*root != NULL);
	assert((*root)->right != NULL);

	// The prior_right node will eventually be saved in root's current position
	struct avl_node *prior_right = (*root)->right;

	// Everything to the left of prior_right is attached to root's right
	(*root)->right = prior_right->left;

	// prior_right's left is now the old root
	prior_right->left = *root;

	// prior_right is the new root
	*root = prior_right;
}

void _rotate_right(struct avl_node **root) {
	assert(root != NULL);
	assert(*root != NULL);
	assert((*root)->left != NULL);

	// The prior_left node will eventually be saved in root's current position
	struct avl_node *prior_left = (*root)->left;

	// Everything to the right of prior_left is attached to root's left
	(*root)->left = prior_left->right;

	// prior_left's left is now the old root
	prior_left->right = *root;

	// prior_left is the new root
	*root = prior_left;
}

void _balance_left(struct avl_node **node, bool *decrease) {
	assert(node != NULL);
	assert(*node != NULL);
	// avl_node_print(*node);
	assert((*node)->left != NULL);

	switch ((*node)->left->balance) {
		case LEFT:
			// left-left tree
			(*node)->balance = BALANCED;
			(*node)->left->balance = BALANCED;
			break;

		case RIGHT:
			// left-right-tree
			assert((*node)->left->right != NULL);
			switch ((*node)->left->right->balance) {
				case BALANCED:
					// left-right-balanced tree
					(*node)->balance = BALANCED;
					(*node)->left->balance = BALANCED;
					break;

				case LEFT:
					// left-right-left tree
					(*node)->balance = RIGHT;
					(*node)->left->balance = BALANCED;
					break;

				case RIGHT:
					// left-right-right tree
					(*node)->balance = BALANCED;
					(*node)->left->balance = LEFT;
					break;
			}

			(*node)->left->right->balance = BALANCED;

			_rotate_left(&(*node)->left);
			break;

		case BALANCED:
			// left-balanced tree
			(*node)->balance = LEFT;
			(*node)->left->balance = RIGHT;
			// The height of the subtree will remain the same after rotating. Occurs only during removals.
			*decrease = false;
			break;
	}

	_rotate_right(node);
}

void _balance_right(struct avl_node **node, bool *decrease) {
	assert(node != NULL);
	assert(*node != NULL);
	assert((*node)->right != NULL);

	switch ((*node)->right->balance) {
		case RIGHT:
			// right-right tree
			(*node)->balance = BALANCED;
			(*node)->right->balance = BALANCED;
			break;

		case LEFT:
			// right-left-tree
			assert((*node)->right->left != NULL);
			switch ((*node)->right->left->balance) {
				case BALANCED:
					// right-left-balanced-tree
					(*node)->balance = BALANCED;
					(*node)->right->balance = BALANCED;
					break;

				case RIGHT:
					// right-left-right tree
					(*node)->balance = LEFT;
					(*node)->right->balance = BALANCED;
					break;

				case LEFT:
					// right-left-left tree
					(*node)->balance = BALANCED;
					(*node)->right->balance = RIGHT;
					break;
			}

			(*node)->right->left->balance = BALANCED;

			_rotate_right(&(*node)->right);
			break;

		case BALANCED:
			// right-balanced tree
			(*node)->balance = RIGHT;
			(*node)->right->balance = LEFT;
			// The height of the subtree will remain the same after rotating. Occurs only during removals.
			*decrease = false;
			break;
	}

	_rotate_left(node);
}

int _add_helper(
    struct avl_node **root, void const *value, void const *data,
    int (*cmp_func)(void const *new_value, void const *node_value), bool *increase) {
	assert(root != NULL);
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

		// printf("\tdirection: %d\n", direction);

		int did_add;
		if (direction <= LEFT) {
			// Add on left
			did_add = _add_helper(&(*root)->left, value, data, cmp_func, increase);
			if (did_add < 0) {
				fprintf(stderr, "_add_helper() error: %d\n", did_add);
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
		} else if (RIGHT <= direction) {
			// Add on right
			did_add = _add_helper(&(*root)->right, value, data, cmp_func, increase);
			if (did_add < 0) {
				fprintf(stderr, "_add_helper() error: %d\n", did_add);
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
				else if (RIGHT < (*root)->balance) {
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

	// We assume that new_value already exists in the tree
	bool increase = false;

	// Obtain exclusive lock over the tree while adding data
	sem_wait(tree->lock);
	int rc = _add_helper(&(tree->root), new_value, new_data, tree->cmp_func, &increase);
	sem_post(tree->lock);

	return rc;
}

int _get_helper(
    struct avl_node *node, void const *value, void const **data,
    int (*cmp_func)(void const *new_value, void const *node_value)) {
	assert(data != NULL);
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
	assert(node_data != NULL);

	// Obtain exclusive lock over the tree while getting data
	sem_wait(tree->lock);
	int rc = _get_helper((tree)->root, search_value, node_data, tree->cmp_func);
	sem_post(tree->lock);

	return rc;
}

int _remove_helper(
    struct avl_node **root, void const *search_value, void const **node_value,
    void const **node_data, int (*cmp_func)(void const *new_value, void const *node_value),
    bool *decrease) {
	assert(root != NULL);
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

		if (direction <= LEFT) {
			// Remove from left side
			did_remove =
			    _remove_helper(&(*root)->left, search_value, node_value, node_data, cmp_func, decrease);

			if (*decrease) {
				// printf("RIGHT for node:\t");
				// avl_node_print(*root);
				// Since a node was removed on the left, the right side is now longer
				(*root)->balance += RIGHT;
				// printf("new balance: %d\n", (*root)->balance);

				// If the overall height of this node did not decrease
				if ((*root)->balance == RIGHT) {
					*decrease = false;
				}
				// If the overall height of this node decreased out of bounds on the right side
				else if (RIGHT < (*root)->balance) {
					_balance_right(root, decrease);
				}
			}

			return did_remove;
		} else if (RIGHT <= direction) {
			// Remove from right side
			did_remove =
			    _remove_helper(&(*root)->right, search_value, node_value, node_data, cmp_func, decrease);

			if (*decrease) {
				// printf("LEFT for node:\t");
				// avl_node_print(*root);
				// Since a node was removed on the right, the left side is now longer
				(*root)->balance += LEFT;
				// printf("new balance: %d\n", (*root)->balance);

				// If the overall height of this node did not decrease
				if ((*root)->balance == LEFT) {
					*decrease = false;
				}
				// If the overall height of this node decreased out of bounds on the left side
				else if ((*root)->balance < LEFT) {
					_balance_left(root, decrease);
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
	assert(node_value != NULL);
	assert(node_data != NULL);

	// We assume that the search_value does not appear in the tree
	bool decrease = false;

	// Obtain exclusive lock while removing data
	sem_wait(tree->lock);
	int rc =
	    _remove_helper(&tree->root, search_value, node_value, node_data, tree->cmp_func, &decrease);
	sem_post(tree->lock);
	return rc;
}

int _avl_subtree_traverse(
    struct avl_node const *root, int (*preorder_func)(struct avl_node const *node, void *arg),
    void *preorder_arg, int (*inorder_func)(struct avl_node const *node, void *arg),
    void *inorder_arg, int (*postorder_func)(struct avl_node const *node, void *arg),
    void *postorder_arg) {
	if (root == NULL) {
		return 0;
	}

	int rc;

	// Apply pre-order function on node
	if (preorder_func != NULL) {
		rc = preorder_func(root, preorder_arg);
		if (rc <= -1) {
			return rc;
		}
	}

	// Go down the left branch
	rc = _avl_subtree_traverse(
	    root->left, preorder_func, preorder_arg, inorder_func, inorder_arg, postorder_func,
	    postorder_arg);
	if (rc <= -1) {
		return rc;
	}

	// Apply in-order function on node
	if (inorder_func != NULL) {
		rc = inorder_func(root, inorder_arg);
		if (rc <= -1) {
			return rc;
		}
	}

	// Go down the right branch
	rc = _avl_subtree_traverse(
	    root->right, preorder_func, preorder_arg, inorder_func, inorder_arg, postorder_func,
	    postorder_arg);
	if (rc <= -1) {
		return rc;
	}

	// Apply post-order function on node
	if (postorder_func != NULL) {
		rc = postorder_func(root, postorder_arg);
		if (rc <= -1) {
			return rc;
		}
	}

	return 0;
}

int avl_tree_traverse(
    struct avl_tree const *tree, int (*preorder_func)(struct avl_node const *node, void *arg),
    void *preorder_arg, int (*inorder_func)(struct avl_node const *node, void *arg),
    void *inorder_arg, int (*postorder_func)(struct avl_node const *node, void *arg),
    void *postorder_arg) {
	assert(tree != NULL);

	sem_wait(tree->lock);
	int rc = _avl_subtree_traverse(
	    tree->root, preorder_func, preorder_arg, inorder_func, inorder_arg, postorder_func,
	    postorder_arg);
	sem_post(tree->lock);
	return rc;
}

void avl_node_print(struct avl_node const *node) {
	printf("(v: %p, d: %p, b: %d)\n", node->value, node->data, node->balance);
}

int _print_node_preorder(struct avl_node const *node, void *depth_p) {
	int64_t depth = *(int64_t *)depth_p;

	for (int64_t d = 0; d < depth; ++d) {
		putchar('\t');
	}
	avl_node_print(node);

	*(int64_t *)depth_p = depth + 1;

	return 0;
}

int _print_node_postorder(struct avl_node const *node __attribute__((unused)), void *depth_p) {
	int64_t depth = *(int64_t *)depth_p;
	*(int64_t *)depth_p = depth - 1;

	return 0;
}

int avl_tree_print(struct avl_tree const *tree) {
	assert(tree != NULL);

	int64_t depth = 0;
	return avl_tree_traverse(
	    tree, _print_node_preorder, &depth, NULL, NULL, _print_node_postorder, &depth);
}
