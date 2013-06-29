/*
 * Implementing searching and sorting subroutines
 */

#include "core.h"

/* binary traversing through the db in order to search
 * see below for multiple modes */
long btraverse(long id, int mode)
{
	long get;
	long middle = 0;
	long search_from = 0;
	long search_to   = db.numberOfBooks - 1;

	while (search_to >= search_from) {
		/*
		 * calculate pivot, based on <mode>. This is to avoid function pointers
		 * 1 - pure binary search
		 * 2 - binary interpolation search
		 */
		if (mode == 1)
			middle = (search_to + search_from) / 2;
		else if (mode == 2) {
			middle = (id - db.arr[search_from].id) * (search_to - search_from);
			middle = (search_from + middle) / (db.arr[search_to].id - db.arr[search_from].id);
			/* out of range is possible in the interpolation formula */
			if (middle < 0)
				middle = -middle;
			if ((unsigned long)middle > db.numberOfBooks)
				middle = db.numberOfBooks;
		} else
			fatal("wrong btraverse mode");

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
