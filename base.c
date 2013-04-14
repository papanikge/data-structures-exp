/**
 * George Papanikolaou 2013
 * Data structures project --- CEID
 * There is absolutely no warranty
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
	char   *summary;
	int    num_authors;
	Author *authors;
} Book;

/* the wrapper around the dynamic array of the managed books */
typedef struct {
	Book *arr;
	int  count;
} Data;

/* panic function for fatal errors */
void fatal(char *message)
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
char get_option()
{
	char in;
	printf("Option? [1..9] ");
	in = (char)getchar();
	return in;
}

/* init db, first menu option function */
void create_db(const char *file)
{
	FILE *fd;
	fd = fopen(file, "r");
	/* TODO read file and create database array */
	fclose(fd);
}

int main(int argc, const char **argv)
{
	/* initialize */
	char filename[50];
	if (argc >= 2)
		strcpy(filename, argv[1]);
	else
		strcpy(filename, "datafile.db");

	/* create_db(filename); */

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
	Book *f;
	f=ec_malloc(sizeof(Book));
	Author a[2];
	a[0].first="Albert";
	a[1].last="Dirac";
	/* a static array in the pointer of the struct inside the struct */
	f->authors = a;
	printf("%s\n",f->authors[1].last);
	free(f);

	/* Done. Finish the job. */
	return 0;
}
