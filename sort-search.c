/*
 * Implementing searching and sorting subroutines
 */

#include "core.h"

/* binary traversing through the db in order to search
 * see below for multiple modes */
int btraverse(long id, int mode)
{
	int middle, get;
	int search_from = 0;
	int search_to   = db.numberOfBooks - 1;

	while (search_to >= search_from) {
		/*
		 * calculate pivot, based on <mode>. This is to avoid function pointers
		 * 1 - pure binary search
		 * 2 - binary interpolation search
		 */
		if (mode == 1)
			middle = (search_to + search_from)/2;
		else {
			middle = search_from +
				((id - db.arr[search_from].id) * (search_to - search_from))/
				(db.arr[search_to].id - db.arr[search_from].id);
		}

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
	Book T;
	int min;
	unsigned long i, j;

	/* implementing selection sort in order to use less memory */
	for (i = 0; i < db.numberOfBooks; i++) {
		min = i;
		for (j = i + 1; j < db.numberOfBooks; j++)
			if (db.arr[j].id < db.arr[min].id)
				min = j;
		/* swap nodes here */
		T = db.arr[i];
		db.arr[i] = db.arr[min];
		db.arr[min] = T;
	}
}
