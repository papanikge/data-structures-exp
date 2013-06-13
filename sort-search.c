/*
 * Implementing searching and sorting subroutines
 */

#include "core.h"

/* the comparing function for two particular Books
 * to be used with glibc qsort function */
static int cmp_nodes(const void *a, const void *b)
{
	Book* A = (Book*) a;
	Book* B = (Book*) b;
	long first = atol(A->id);
	long second = atol(B->id);

	return (int)(first - second);
}

/* Main sort function. The implementations may vary */
void sort_db(void)
{
	/* using libc's Quicksort since we favor speed over memory */
	qsort(&db.arr[0], db.numberOfBooks, sizeof(Book), cmp_nodes);
}

/* Binary search database for a given id
 * currently uses libc's bsearch function
 * Caller *must sort* db
 * Return value may be NULL */
Book* bsearch_by_id(const char* id)
{
	Book key;
	strncpy(key.id, id, 11);
	return bsearch(&key, &db.arr[0], db.numberOfBooks, sizeof(Book), cmp_nodes);
}
