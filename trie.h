/* Trie header file */

#ifndef TRIE_H_INCLUDED
#define TRIE_H_INCLUDED

typedef struct TrieNode {
	char key;
	/* value that the node references (not NULL if node is a leaf) */
	Book* value;
	/* pointers to children (26 letters of latin alphabet) */
	struct TrieNode *edges[26];
} TrieNode;

/* prototypes */
Book*     trie_find(char *str, TrieNode *t);
TrieNode* trie_initialize(char key);
TrieNode* trie_insert(char *str, Book* v, TrieNode *t);
TrieNode* trie_delete(char *str, TrieNode *t);
void trie_dispose(TrieNode* t);

#endif
