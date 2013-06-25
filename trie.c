/*
 * George 'papanikge' Papanikolaou CEID Data Structures 2013
 * Simple Trie Implementation
 * See other files about licensing
 */

/*
 * Tries are represented by the TrieNode which have a value and a (latin
 * alphabet size) array of pointers to children (not compressed).
 * Functions work by traversing the string one by one character
 */

#include "core.h"
#include "trie.h"

/* memory allocation and assignments to new node */
TrieNode* trie_initialize(char key)
{
	int i;
	TrieNode *node;

	node = (TrieNode*) smalloc(sizeof(TrieNode));
	node->key   = key;
	node->value = NULL;
	/* null-ing all nodes */
	for (i = 0; i < 26; i++)
		node->edges[i] = NULL;
	return node;
}

/* Add a key to the trie and create nodes as needed (recursively) */
TrieNode* trie_insert(char *str, Book* v, TrieNode *t)
{
	int index;

	/* safety */
	if (!t)
		return NULL;

	if (str[0] == '\0') {
		/* reached the end */
		t->key   = '\0';
		t->value = v;
	} else {
		/* get index in 0..25 */
		index = str[0] - 'a';
		/* check for existence */
		if(!t->edges[index])
			t->edges[index] = trie_initialize(str[0]);
		/* key, for later */
		t->key = str[0];
		/* shift the head of the string to continue recursing */
		str++;
		t->edges[index] = trie_insert(str, v, t->edges[index]);
	}
	return t;
}

/* traverse the string and delete if there is nothing else (recursively) */
TrieNode* trie_delete(char *str, TrieNode *t)
{
	int i;
	int index;
	/* true by default */
	int all_null = 1;
	/* optimization flag in case of non-existence */
	int no_need = 0;

	/* safety */
	if (!t)
		return NULL;

	if (str[0] == '\0') {
		/* reached it */
		free(t);
		return NULL;
	} else {
		/* get index in 0..25 */
		index = str[0] - 'a';
		/* if not found, we just return */
		if(!t->edges[index]) {
			no_need = 1;
			return NULL;
		}
		/* shift the head of the string to continue recursing */
		str++;
		t->edges[index] = trie_delete(str, t->edges[index]);
		/* we need to delete all the nodes that lead to the one we removed
		 * if there are no other children */
		if (no_need)
			return NULL;
		for (i = 0; i < 26; i++)
			if (t->edges[i])
				all_null = 0;

		if (all_null) {
			free(t);
			return NULL;
		}
		else
			return t;
	}
}

/* traverse the trie and return the value if found (recursively) */
Book* trie_find(char *str, TrieNode *t)
{
	Book* b;
	int index;

	/* safety */
	if (!t)
		return NULL;

	if (str[0] == '\0')
		return t->value;
	else {
		/* get index in 0..25 */
		index = str[0] - 'a';
		/* if not found, we need to return NULL up the stack */
		if(!t->edges[index])
			return NULL;
		/* shift the head of the string to continue recursing */
		str++;
		b = trie_find(str, t->edges[index]);
		/* return whatever came back, Book or NULL */
		return b;
	}
}

/* helper for when closing up (recursively) */
void trie_dispose(TrieNode* t)
{
	int i;

	if (t) {
		for (i = 0; i < 26; i++)
			trie_dispose(t->edges[i]);
		free(t);
	}
	return;
}
