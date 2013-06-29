/**
 * This is the main file for the Data Structures project (CEID)
 *
 * Copyright 2013 George "papanikge" Papanikolaou
 *
 * This is experimental software. Beware of Dragons.
 *
 * This project is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with these files; If not, see <http://www.gnu.org/licenses/>.
 **/

#include "core.h"
#include "avl.h"
#include "trie.h"
#include <sys/time.h>

/* global main in-memory dynamic database and accompanied structures */
Data db;
AvlNode *avl;
TrieNode *trie_title;
TrieNode *trie_name;
/* is the db sorted? */
int sorted = 0;

/* panic function for fatal errors */
inline void fatal(char *message)
{
	fprintf(stderr, "\a*** Fatal error: %s\n", message);
	exit(EXIT_FAILURE);
}

/* a fail-aware malloc function */
inline void* smalloc(const unsigned int size)
{
	void *ptr;
	ptr = malloc(size);
	if(!ptr)
		fatal("on memory allocation");
	memset(ptr, 0, size);
	return ptr;
}

/* consume a possible newline in standard input */
inline void clear_stream(void)
{
	int tmp = fgetc(stdin);
	if (tmp != '\n')
		ungetc(tmp, stdin);
}

/* calculate time deltas from a starting point */
static void calc_delta(struct timeval *start, double *sec, double *usec)
{
	struct timeval end;

	gettimeofday(&end, NULL);
	*sec  = ((end.tv_sec  - start->tv_sec)); 
	*usec = ((end.tv_usec - start->tv_usec)); 
	/* start micro seconds may be bigger */
	if (*usec < 0)
		*usec = -(*usec);
}

/* get a valid option from user */
static int get_option(void)
{
	int in = 0;
	do {
		printf("Option? [1..9] ");
		scanf("%d", &in);
		/* consume extra chars */
		while(getchar() != '\n');
	} while (in < 1 || in > 9);
	return in;
}

/* calling when creating the db all over again or when quitting */
static void db_dispose(void)
{
	unsigned long i;
	for (i = 0; i < db.numberOfBooks; i++)
		free(db.arr[i].authors);
	free(db.arr);
}

/* Linear search (menu option 5) */
static void search_by_id(void)
{
	long id;
	int way;
	long index;
	Book *B;

	printf("ID to search for? ");
	clear_stream();
	scanf("%ld", &id);

	printf("Which way to search? [1-linear 2-binary 3-interpolation] ");
	clear_stream();
	scanf("%d", &way);

	switch(way) {
		case 1:
			index = find_index_by_id(id);
			break;
		case 2:
		case 3:
			/* sort if needed */
			if (!sorted) {
				printf("Sorting...\n");
				sort_db();
				sorted = 1;
			}
			/* using way - 1 here to get the correct mode in btraverse */
			index = btraverse(id, way - 1);
			break;
		default:
			printf("Wrong answer\n");
			return;
	}

	if (index == -1) {
		printf("Book not found\n");
		return;
	}
	B = &db.arr[index];

	printf("Title : %s\n",    B->title);
	printf("Author: %s %s\n", B->authors[0].first, B->authors[0].last);
	printf("Year  : %d\n",    B->yearPublished);
}

/* Linear search given a title (menu option 6) */
static void search_by_title(void)
{
	unsigned long i;
	char title[256];

	printf("Title of Book to (linearly) search for? ");

	clear_stream();
	scanf("%[0-9a-zA-Z.:!'?,)( ]", title);

	for (i = 0; i < db.numberOfBooks; i++) {
		if (strcmp(db.arr[i].title, title) == 0) {
			printf("ID    : %ld\n",   db.arr[i].id);
			printf("Author: %s %s\n", db.arr[i].authors[0].first, db.arr[i].authors[0].last);
			printf("Year  : %d\n",    db.arr[i].yearPublished);
			return;
		}
	}
	printf("No Book with that title\n");
	return;
}

/* Linear search given a surname of author (menu option 8) */
static void search_by_surname(void)
{
	unsigned long i;
	char surname[56];
	short flag = 1;

	printf("Surname of Author? ");

	clear_stream();
	scanf("%[a-zA-Z. ]", surname);

	for (i = 0; i < db.numberOfBooks; i++) {
		if (strcmp(db.arr[i].authors[0].last, surname) == 0) {
			flag = 0;
			printf("ID  : %ld\n", db.arr[i].id);
			printf("Name: %s\n", db.arr[i].title);
			printf("Year: %d\n\n", db.arr[i].yearPublished);
		}
	}
	if (flag)
		printf("No Book with such Author\n");
	return;
}

/* menu option 4 */
static void user_remove_book(void)
{
	char title[256];
	long id;
	long index;

	printf("ID of the book to remove? ");

	clear_stream();
	scanf("%ld", &id);

	/* Linear search */
	index = find_index_by_id(id);
	if (index == -1) {
		printf("Book not found\n");
		return;
	}

	/* keep the title for later */
	strcpy(title, db.arr[index].title);

	if (!remove_book(index))
		printf("\"%s\" removed from database\n", title);
	else
		printf("Couldn't reallocate\n");

	return;
}

int main(int argc, const char **argv)
{
	int k;
	unsigned int i;
	double sec, usec;
	struct timeval start, end;
	char filename[50];
	/* handle data base file name (not safe) */
	if (argc >= 2)
		strcpy(filename, argv[1]);
	else
		strcpy(filename, "datafile");

	avl  = NULL;
	/* we need \0 to be an empty trie */
	trie_title = trie_initialize('\0');
	trie_name  = trie_initialize('\0');

	init_db(filename);

	/* initialize avl tree (for id) and tries */
	for (i = 0; i < db.numberOfBooks; i++) {
		avl = avl_insert(&db.arr[i], avl);
		trie_title = trie_insert(db.arr[i].title, &db.arr[i], trie_title);
		trie_name  = trie_insert(db.arr[i].authors[0].last, &db.arr[i], trie_name);
	}

	printf(" -== User Interactions here ==-\nTimes:\n");

	/* linear search */
	gettimeofday(&start, NULL);
		for (k = 0; k < 1000; k++)
			find_index_by_id(k * 3083);
	calc_delta(&start, &sec, &usec);
	printf("\tLinear: %1.f,%1.f sec\n", sec, usec);

	/* binary search */
	gettimeofday(&start, NULL);
		for (k = 0; k < 1000; k++)
			btraverse(k * 3083, 1);
	calc_delta(&start, &sec, &usec);
	printf("\tBinary: %1.f,%1.f sec\n", sec, usec);

	/* binary interpollation search */
	gettimeofday(&start, NULL);
		for (k = 0; k < 1000; k++)
			btraverse(k * 3083, 2);
	calc_delta(&start, &sec, &usec);
	printf("\tBIS:    %1.f,%1.f sec\n", sec, usec);

	/* avl trees search */
	gettimeofday(&start, NULL);
		for (k = 0; k < 1000; k++)
			avl_find(k * 3083, avl);
	calc_delta(&start, &sec, &usec);
	printf("\tAVL:    %1.f,%1.f sec\n", sec, usec);

	/* tries title search */
	gettimeofday(&start, NULL);
		for (k = 0; k < 1000; k++) {
			if (k % 2)
				/* not present */
				trie_find("Einmal ist keeeeeim", trie_title);
			else
				/* present */
				trie_find("Andamios", trie_title);
		}
	calc_delta(&start, &sec, &usec);
	printf("\tTries1: %1.f,%1.f sec (with titles)\n", sec, usec);

	/* tries name search */
	gettimeofday(&start, NULL);
		for (k = 0; k < 1000; k++) {
			if (k % 2)
				/* not present */
				trie_find("bqqbbqqbbqqbqbb", trie_name);
			else
				/* present */
				trie_find("Hall", trie_name);
		}
	calc_delta(&start, &sec, &usec);
	printf("\tTries2: %1.f,%1.f sec (with names)\n", sec, usec);

	/* free, quit */
	avl_dispose(avl);
	trie_dispose(trie_title);
	trie_dispose(trie_name);
	db_dispose();
	return 0;
}
