/**
 * Books data structures CEID project Basic header file
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/******************************* Base Structs ********************************/

/* author type for names */
typedef struct {
	char first[56];
	char last[56];
} Author;

/* a single unit type */
/* more than one author possible */
typedef struct {
	long   id;
	char   title[256];
	char   publisher[40];
	short  yearPublished;
	int    numberOfAuthors;
	Author *authors;
} Book;

/* the wrapper around the dynamic array of the managed books */
typedef struct {
	Book  *arr;
	unsigned long  numberOfBooks;
} Data;

/*********************** Global function prototypes **************************/

/* global main in-memory dynamic database */
extern Data db;
extern int sorted;

inline void fatal(char *message);
inline void* smalloc(const unsigned int size);
inline void clear_stream(void);
int add_book(Book* A);
int remove_book(long index);
void init_db(const char *file);
void print_db(const char *file);
void user_add_book(void);
void sort_db(void);
void split_create_authors(Book* B, char* full);
long find_index_by_id(long id);
long btraverse(long id, int mode);
