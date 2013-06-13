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

/* global main in-memory dynamic database */
Data db;

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
		printf("\nOption? [1..9] ");
		in = getchar();
		/* getting only the first and cleaning the rest */
		while(getchar() != '\n');
	} while (in < (int)'1' || in > (int)'9');
	/* damn you ascii */
	return (in - 48);
}

/* calling when creating the db all over again or when quitting */
static void free_db(void)
{
	unsigned long i;
	for (i = 0; i < db.numberOfBooks; i++)
		free(db.arr[i].authors);
	free(db.arr);
}

/* Linear search (menu option 5) */
static void search_by_id(void)
{
	char id[11];
	int way;
	long index;
	Book *B;

	printf("ID to search for? ");
	clear_stream();
	scanf("%[0-9]", id);

	printf("Which way to search? [1-linear 2-binary] ");
	clear_stream();
	scanf("%d", &way);

	switch(way) {
		case 1:
			index = find_index_by_id(id);
			if (!index) {
				printf("Book not found\n");
				return;
			}
			B = &db.arr[index];
			break;
		case 2:
			sort_db();
			B = bsearch_by_id(id);
			if (B == NULL) {
				printf("Book not found\n");
				return;
			}
			break;
		default:
			printf("Wrong answer\n");
			return;
	}

	printf("Title : %s\n",    B->title);
	printf("Author: %s %s\n", B->authors[0].first,
							  B->authors[0].last);
	printf("Year  : %d\n",    B->yearPublished);
}

/* Linear search given a title (menu option 6) */
static void search_for_title(void)
{
	unsigned long i;
	char title[256];

	printf("Title of Book to search for? ");

	clear_stream();
	scanf("%[0-9a-zA-Z.:!'?,)( ]", title);

	for (i = 0; i < db.numberOfBooks; i++) {
		if (!strcmp(db.arr[i].title, title)) {
			printf("ID    : %s\n",    db.arr[i].id);
			printf("Author: %s %s\n", db.arr[i].authors[0].first, db.arr[i].authors[0].last);
			printf("Year  : %d\n",    db.arr[i].yearPublished);
			return;
		}
	}
	printf("No Book with that title\n");
	return;
}

/* Linear search given a surname of author (menu option 8) */
static void search_for_surname(void)
{
	unsigned long i;
	char surname[56];
	short flag = 1;

	printf("Surname of Author? ");

	clear_stream();
	scanf("%[a-zA-Z. ]", surname);

	for (i = 0; i < db.numberOfBooks; i++) {
		if (!strcmp(db.arr[i].authors[0].last, surname)) {
			flag = 0;
			printf("ID  : %s\n", db.arr[i].id);
			printf("Name: %s\n", db.arr[i].title);
			printf("Year: %d\n\n", db.arr[i].yearPublished);
		}
	}
	if (flag)
		printf("No Book with such Author\n");
	return;
}

int main(int argc, const char **argv)
{
	unsigned int opt;
	/* handle data base file name */
	char filename[50];
	if (argc >= 2)
		strcpy(filename, argv[1]);
	else
		strcpy(filename, "datafile");

	init_db(filename);

	printf("[1] Load books from file\n");
	printf("[2] Save books to file\n");
	printf("[3] Add a book\n");
	printf("[4] Delete a book by id\n");
	printf("[5] Display a book by id\n");
	printf("[6] Display a book by title\n");
	printf("[7] Display books\n");
	printf("[8] Display books by surname search\n");
	printf("[9] Exit\n");

	/* main loop */
	while ((opt = get_option()) != 9) {
		switch (opt) {
			case 1:
				free_db();
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
				search_for_title();
				break;
			case 7:
				print_db("stdout");
				break;
			case 8:
				search_for_surname();
				break;
			default:
				fatal("get_option() returns something wrong");
				break;
		}
	}

	/* got exit command. save and quit */
	/* print_db(filename); */
	free_db();
	return 0;
}
