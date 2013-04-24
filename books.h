/**
 * Books data structures CEID project header file
 */

/* maximum sizes (included pointed strings) */
#define MAXAUTHOR 100
#define MAXTITLE 256
#define MAXPUBL  40
#define MAXENTRY (MAXTITLE + MAXPUBL + MAXAUTHOR + sizeof(Book))

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

/* panic function for fatal errors */
inline void fatal(char *message)
{
	fprintf(stderr, "\a*** Fatal error: %s\n", message);
	exit(EXIT_FAILURE);
}

/* a fail-aware malloc function */
void* smalloc(const unsigned int size)
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
