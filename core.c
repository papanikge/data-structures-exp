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
	unsigned int i, opt;
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

	printf(" [1] Load books from file\n"
			" [2] Save books to file\n"
			" [3] Add a book\n"
			" [4] Delete a book by id\n"
			" [5] Display a book by id\n"
			" [6] Display a book by title\n"
			" [7] Display books\n"
			" [8] Display books by surname search\n"
			" [9] Exit\n");

	/* main loop */
	while ((opt = get_option()) != 9) {
		switch (opt) {
			case 1:
				free(db.arr);
				init_db(filename);
				break;
			case 2:
				print_db(filename);
				break;
			case 3:
				user_add_book();
				break;
			case 4:
				user_remove_book();
				break;
			case 5:
				search_by_id();
				break;
			case 6:
				search_by_title();
				break;
			case 7:
				print_db("stdout");
				break;
			case 8:
				search_by_surname();
				break;
			default:
				fatal("get_option() returns something wrong");
				break;
		}
	}

	/* got exit command. save and quit */
	/* print_db(filename); */
	/* free memory */
	avl_dispose(avl);
	trie_dispose(trie_title);
	trie_dispose(trie_name);
	free(db.arr);
	return 0;
}
