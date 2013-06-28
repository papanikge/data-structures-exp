/* AVL tree header file */

#ifndef AVLTREE_H_INCLUDED
#define AVLTREE_H_INCLUDED

typedef struct AvlNode {
	/* The actual data of the node. avl trees have data in all nodes */
	Book* cont;
	/* pointers to the children */
	struct AvlNode *left;
	struct AvlNode *right;
	/* longest distance to the leaf */
	int height;
} AvlNode;

/* prototypes */
void avl_dispose(AvlNode* t);
Book* avl_find(const long f, AvlNode* t);
AvlNode* avl_insert(Book* b, AvlNode* t);
AvlNode* avl_delete(const long b, AvlNode* t);

#endif
