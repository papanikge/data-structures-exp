/*
 * George 'papanikge' Papanikolaou CEID Data Structures 2013
 * simple AVL tree implementation (based on zentut.com)
 */

/*
 * Trees are represented with the AvlNode struct which points to the
 * correct children and so on and so forth. There is no different
 * structure for node and tree
 */

#include "core.h"
#include "avl.h"

/**************************** Auxilary functions *****************************/

/* get the height of a node if exists or -1 (used as a balance factor) */
static inline int calc_height(AvlNode* n)
{
	if (!n)
		return -1;
	else
		return n->height;
}

/* keep biggest value of two integers */
static inline int max(const int left, const int right)
{
	return (left > right) ? left : right;
}

/* find maximum node's key */
static AvlNode* avl_find_max(AvlNode* t)
{
	if (t)
		while (t->right != NULL)
			t = t->right;
	return t;
}

/* find a specific node's key in the tree (recursively) */
static AvlNode* avl_find_node(const long f, AvlNode* t)
{
	if (!t)
		return NULL;
	if (f < t->cont->id)
		return avl_find_node(f, t->left);
	else if (f > t->cont->id)
		return avl_find_node(f, t->right);
	else
		/* found it */
		return t;
}

/*
 * Rotations. Note:
 * These functions do *not* check for child existence
 */

/*
 * Rotate a node (k2) with its left child (Right rotation)
 *             +------+
 *    +~~~~~>  |  k2  |
 * (2)'        +------+
 *    '        |     
 *    '  +-----+-----+
 *    "  |           '(1)
 *   +------+        '
 *   |  k1  |        '
 *   +------+        '
 *          |        '
 *          +-----+  '
 *                |  "
 *                +-----+
 *                |     |
 *                +-----+
 */
static AvlNode* right_rotation(AvlNode* k2)
{
	/* create a node */
	AvlNode *k1;

	/* shortcut and return value for later */
	k1 = k2->left;
	/* step (1) */
	/* we may as well: k2->left = k2->left->right; */
	k2->left = k1->right;
	/* step (2) */
	k1->right = k2;

	/* calculate height for the new nodes */
	k2->height = max(calc_height(k2->left), calc_height(k2->right)) + 1;
	k1->height = max(calc_height(k1->left), k2->height) + 1;

	/* return new subtree root */
	return k1;
}

/* Rotate a node (k2) with its right child (Left Rotation) */
static AvlNode* left_rotation(AvlNode* k1)
{
	AvlNode *k2;

	/* the other way around as above */
	k2 = k1->right;
	k1->right = k2->left;
	k2->left = k1;

	k1->height = max(calc_height(k1->left), calc_height(k1->right)) + 1;
	k2->height = max(calc_height(k2->right), k1->height) + 1;

	/* return new subtree root */
	return k2;
}

/* double rotations using the above functions */

static AvlNode* left_right_rotation(AvlNode* k3)
{
	/* Rotate between k1 and k2 */
	k3->left = left_rotation(k3->left);

	/* Rotate between K3 and k2 */
	return right_rotation(k3);
}

static AvlNode* right_left_rotation(AvlNode* k1)
{
	/* rotate between K3 and k2 */
	k1->right = right_rotation(k1->right);

	/* rotate between k1 and k2 */
	return left_rotation(k1);
}

/**************************** Exported functions *****************************/

/* insert a new node into the tree (recursively) */
AvlNode* avl_insert(Book* b, AvlNode* t)
{
	int balance_factor;

	/* node creation */
	if (!t) {
		t = (AvlNode *) smalloc(sizeof(AvlNode));
		/* node initialization */
		t->cont = b;
		t->height = 0;
		t->left  = NULL;
		t->right = NULL;
	}
	/* left subtree */
	else if (b->id < t->cont->id) {
		t->left = avl_insert(b, t->left);
		/* Rebalance after return.
		 * Balance factor optimised so it can only be positive */
		balance_factor = calc_height(t->left) - calc_height(t->right);
		if (balance_factor == 2) {
			if (b->id < t->left->cont->id)
				/* wikipedia checks for this to rotate left:
				 * if (balance_factor(L) > 0)
				 * and then always rotate_right(P); */
				t = right_rotation(t);
			else
				t = left_right_rotation(t);
		}
	}
	/* right subtree */
	else if (b->id > t->cont->id) {
		t->right = avl_insert(b, t->right);
		/* Rebalance after return.
		 * Balance factor optimised so it can only be positive */
		balance_factor = calc_height(t->right) - calc_height(t->left);
		if (balance_factor == 2) {
			if (b->id > t->right->cont->id)
				t = left_rotation(t);
			else
				t = right_left_rotation(t);
		}
	}
	/* else Book is already present */

	/* need to calculate height */
	t->height = max(calc_height(t->left), calc_height(t->right)) + 1;
	/* return the new tree */
	return t;
}

/* remove a node in the tree (recursively) */
AvlNode* avl_delete(const long b, AvlNode* t)
{
	AvlNode *m;
	int balance_factor;

	if (b == t->cont->id) {
		/* find biggest value of left subtree and swap */
		if (t->height > 1) {
			m = avl_find_max(t->left);
			t->cont = m->cont;
			/* delete that one */
			free(m);
		} else
			free(t);
		return NULL;
	}
	else if (b < t->cont->id) {
		t->left = avl_delete(b, t->left);
		balance_factor = calc_height(t->left) - calc_height(t->right);
		if (balance_factor == 2) {
			if (b < t->left->cont->id)
				t = right_rotation(t);
			else
				t = left_right_rotation(t);
		}
		t->height = max(calc_height(t->left), calc_height(t->right)) + 1;
		return t;
	}
	else if (b > t->cont->id) {
		t->right = avl_delete(b, t->right);
		balance_factor = calc_height(t->right) - calc_height(t->left);
		if (balance_factor == 2) {
			if (b > t->right->cont->id)
				t = left_rotation(t);
			else
				t = right_left_rotation(t);
		}
		t->height = max(calc_height(t->left), calc_height(t->right)) + 1;
		return t;
	}
	/* else */
	return NULL;
}

/* return content for found node */
Book* avl_find(const long f, AvlNode* t)
{
	AvlNode *n;
	n = avl_find_node(f, t);
	if (n)
		return n->cont;
	else
		return NULL;
}

/* remove and free all nodes of an AVL tree (recursively) */
void avl_dispose(AvlNode* t)
{
	if (t) {
		avl_dispose(t->left);
		avl_dispose(t->right);
		free(t);
	}
}
