/**
 * George Papanikolaou 2013
 * Data structures project --- CEID
 * There is Absolutely NO Warranty.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* maximum sizes (included pointed strings) */
#define MAXAUTHOR 100
#define MAXTITLE 256
#define MAXPUBL  40
#define MAXENTRY (MAXTITLE + MAXPUBL + sizeof(Book))

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
	long   id;
	char   *title;
	char   *publisher;
	short  yearPublished;
	int    numberOfAuthors;
	Author *authors;
} Book;

/* the wrapper around the dynamic array of the managed books */
typedef struct {
	Book  *arr;
	long  numberOfBooks;
} Data;

/* global main in-memory dynamic database */
Data db;

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

/* load and initialize db from file first menu option function */
void init_db(const char *file)
{
	/* temp buffers and descriptors */
	FILE *fd;
	short i;   /* for the temp index */
	short j;   /* for the line index */
	char line[MAXENTRY];
	char book_name[MAXTITLE];
	char writer[MAXAUTHOR];
	char first_name[MAXAUTHOR];
	char last_name[MAXAUTHOR];
	char the_year[4];
	char pub_name[MAXPUBL];
	fd = fopen(file, "r");
	if (!fd)
		fatal("while opening data file");
	/* get the total number of books */
	if (!fgets(line, 8, fd))
		fatal("while reading data file");
	long n = atol(line);
	/* allocate the main database */
	db.arr = ec_malloc(MAXENTRY*n);
	db.numberOfBooks = n;
	/* start parsing the file, delimiter is ';' */
	while (fgets(line, MAXENTRY, fd) != NULL) {
		j = 0;
		/* first the name */
		i = 0;
		while (line[j] != ';') {
			/* don't wrap my strings in "" */
			if (line[j] == '"') {
				j++;
				continue;
			}
			book_name[i] = line[j];
			i++;
			j++;
		}
		j++;
		/* second the author */
		i = 0;
		while (line[j] != ';') {
			if (line[j] == '"') {
				j++;
				continue;
			}
			writer[i] = line[j];
			i++;
			j++;
		}
		j++;
		/* next the year */
		i = 0;
		while (line[j] != ';') {
			if (line[j] == '"') {
				j++;
				continue;
			}
			the_year[i] = line[j];
			i++;
			j++;
		}
		j++;
		/* and last one is the publisher */
		i = 0;
		while (line[j] != '\n') {
			if (line[j] == '"') {
				j++;
				continue;
			}
			pub_name[i] = line[j];
			i++;
			j++;
		}
		/* done. final work */
		Book *B;
		B = create_book(book_name, atoi(the_year), pub_name);
		/* splitting names. (Multiple authors separated with commas) */
		j = 0;  /* using `i` and `j` as indexes again to save memory */
next:
		/**
		 * this is the entry point for every author loop, implemented with
		 * a goto label to save multiple nested while loops
		 */
		i = 0;
		while (writer[j] != '\0' && writer[j] != ',' && writer[j] != ' ') {
			first_name[i] = writer[j];
			j++;
			i++;
		}
		/* if we got a space, we go for last name */
		if (writer[j] == ' ') {
			i = 0;
			j++;
			while (writer[j] != '\0' && writer[j] != ',') {
				last_name[i] = writer[j];
				j++;
				i++;
			}
		}
		/* add him */
		add_author(B, first_name, last_name);
		if (writer[j] == ',') {
			/* with comma, we go for second author */
			j++;
			goto next;
		}

		/* put it in the actual database */
		db.arr[B->id] = *B;
		/* the id is incremented inside create_book() */
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

	printf("Loading file...\n");
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
