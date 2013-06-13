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

/* binary traversing through the db in order to search
 * see below for multiple modes */
static int btraverse(Book *key, int mode)
{
	int middle, get;
	int search_from = 0;
	int search_to = db.numberOfBooks - 1;

	while (search_to >= search_from) {
		/*
		 * calculate pivot, based on <mode>. This is to avoid function pointers
		 * 1 - pure binary search
		 * 2 - binary interpolation search
		 */
		if (mode == 1)
			middle = (search_to - search_from)/2 + search_from;
		else {/* calculate pivot */}

		get = cmp_nodes(key, &db.arr[middle]);

		if (get > 0)
			search_from = middle + 1;
		else if (get < 0)
			search_to = middle - 1;
		else
			return middle;
	}
	return -1;
}

/* Main sort function. The implementations may vary */
void sort_db(void)
{
	/* using libc's Quicksort since we favor speed over memory */
	qsort(&db.arr[0], db.numberOfBooks, sizeof(Book), cmp_nodes);
}

/* Binary search database for a given id
 * currently using btraverse with mode 1
 * Caller *must sort* db
 * Return value may be NULL */
Book* bsearch_by_id(const char* id)
{
	int ret;
	Book key;
	strncpy(key.id, id, 11);

	ret = btraverse(&key, 1);
	return (ret == -1) ? NULL : &db.arr[ret];
}
