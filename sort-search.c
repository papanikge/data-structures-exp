/*
 * Implementing searching and sorting subroutines
 */

#include "core.h"

/* plain binary search with upper and lower limit */
long bsearch_by_id(long id)
{
	long get;
	long middle = 0;
	long search_from = 0;
	long search_to   = db.numberOfBooks - 1;

	while (search_to >= search_from) {
		middle = (search_to + search_from) / 2;

		/* a way to find out which is bigger */
		get = id - db.arr[middle].id;

		if (get > 0)
			search_from = middle + 1;
		else if (get < 0)
			search_to = middle - 1;
		else
			return middle;
	}
	return -1;
}

/* binary interpolation search */
long bis_by_id(long id)
{
	long get;
	long m1, m2, m3; /* helpers for pivot */
	long middle = 0;
	long search_from = 0;
	long search_to   = db.numberOfBooks - 1;

	/* while (search_to > search_from) { */
	while (db.arr[search_from].id <= id && db.arr[search_to].id >= id) {
		m1 = (search_to - search_from) * (id - db.arr[search_from].id);
		m2 = db.arr[search_to].id - db.arr[search_from].id;
		m3 = m1/m2;
		middle = search_from + m3;
		/* out of range is possible in the interpolation formula */
		if (middle < 0)
			middle = -middle;
		else if (middle > db.numberOfBooks)
			middle = db.numberOfBooks;

		/* a way to find out which is bigger */
		get = id - db.arr[middle].id;

		if (get > 0)
			search_from = middle + 1;
		else if (get < 0)
			search_to = middle - 1;
		else
			return middle;
	}
	/* don't follow the same way if searching one cell */
	if (id == db.arr[search_from].id)
		return search_from;
	else
		return -1;
}

/* Main sort function. The implementations may vary */
void sort_db(void)
{
	Book pivot;
	long i, j;

	/* this is insertion sort. it may still be slow */
    for (i = 1; (unsigned)i < db.numberOfBooks; i++) {
		/* keep the pivot element to check and have it available later */
        pivot = db.arr[i];
		/* iterate from i and back. pushing the other */
        for (j = i -1; j >= 0 && pivot.id < db.arr[j].id; j--)
            db.arr[j + 1] = db.arr[j];
		/* found the right place, write the saved */
        db.arr[j + 1] = pivot;
    }
}
