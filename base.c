/**
 * George Papanikolaou 2013
 * Data structures project --- CEID
 * There is absolutely no warranty
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* maximum book struct size (included pointed strings) */
#define MAXENTRY 300

/* for the individual ids of book structs */
long idSum = 0;

/* author type for names */
typedef struct {
	char *first;
	char *last;
} Author;

/* a single unit type */
/* more than one author possible */
typedef struct {
	int    id;
	char   *title;
	char   *publisher;
	short  yearPublished;
	int    numberOfAuthors;
	Author *authors;
} Book;

/* the wrapper around the dynamic array of the managed books */
typedef struct {
	Book *arr;
	long  numberOfBooks;
} Data;

/* panic function for fatal errors */
inline void fatal(char *message)
{
	fprintf(stderr, "\a*** Fatal error: %s\n", message);
	exit(EXIT_FAILURE);
}

/* a fail-aware malloc function */
void* ec_malloc(const unsigned int size)
{
	void *ptr;
	ptr = malloc(size);
	if(!ptr)
		fatal("on memory allocation");
	return ptr;
}

/* get a valid option from user */
int get_option(void)
{
	int in;
	do {
		printf("\nOption? [1..9] ");
		in = getchar();
	} while (in < 48 || in > 57);
	/* damn you ascii */
	return (in - 48);
}

/* create and return a book struct */
Book* create_book(char* name, short year, char* pub)
{
	Book *b = ec_malloc(sizeof(Book));
	b->id = idSum;
	idSum++;
	/* the rest */
	b->title = name;
	b->publisher = pub;
	b->yearPublished = year;
	b->numberOfAuthors = 0;
	return b;
}

/* dynamically change the authors array in a given book struct */
void add_author(Book* b, char* f, char* l)
{
	Author *a;
	short index = b->numberOfAuthors;
	/* creating or reallocating the proper array */
	if (index == 0) {
		a = ec_malloc(sizeof(Author));
	} else {
		a = realloc(b->authors, (sizeof(Author)*(index+1)) );
		if (!a) fatal("while reallocating to add author");
	}
	/* now add the names and point back */
	a[index].first = f;
	a[index].last  = l;
	b->authors = a;
	/* increment the counter */
	b->numberOfAuthors++;
}

/* init db, first menu option function */
void init_db(const char *file)
{
	FILE *fd;
	char buf[MAXENTRY];
	fd = fopen(file, "r");
	while (fgets(buf, MAXENTRY, fd) != NULL) {
		/* code */
	}
	fclose(fd);
}

int main(int argc, const char **argv)
{
	/* handle data base file name */
	char filename[50];
	if (argc >= 2)
		strcpy(filename, argv[1]);
	else
		strcpy(filename, "datafile.db");

	/* init_db(filename); */

	printf("[1] Load books from file\n");
	printf("[2] Save books to file\n");
	printf("[3] Add a book\n");
	printf("[4] Delete a book by id\n");
	printf("[5] Display a book by id\n");
	printf("[6] Display a book by title\n");
	printf("[7] Display books\n");
	printf("[8] Display books by surname search\n");
	printf("[9] Exit\n");

	/* testing the array inside of the structs */
	Book *b = create_book("Digital Fortress", "I have no idea");
	add_author(b, "Dan", "Brown");
	printf("is it Dan? %s\n", b->authors[0].first);

	/* main loop */
	short opt;
	while ((opt = get_option()) != 9) {
		switch (opt) {
			case 1:
				/* init_db(filename); */
				break;
			case 2:
				/* save_db(); */
				break;
			case 3:
				/* add a book */
				break;
			case 4:
				/* remove a book */
				break;
			case 5:
				/* search by id */
				break;
			case 6:
				/* search by name */
				break;
			case 7:
				/* display all */
				break;
			case 8:
				/* search by last name of author */
				break;
			default:
				fatal("get_option() returns something wrong");
				break;
		}
	}

	/* got exit command. save and quit */
	/* save_db(); */
	return 0;
}
