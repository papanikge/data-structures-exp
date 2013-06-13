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

	if (first == second)
		return 0;
	/* else */
	return (first > second) ? 1 : -1;
}

/* Main sort function. The implementations may vary */
void sort_db(void)
{
	/* using libc's Quicksort since we favor speed over memory */
	qsort(&db.arr[0], db.numberOfBooks, sizeof(Book), cmp_nodes);
}
