/**
 * This file is part of the Data Structures project (CEID)
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
 * You should have received a copy of the GNU Lesser General Public License
 * along with these files; If not, see <http://www.gnu.org/licenses/>.
 */

/* 
 * Implementing the database actions such as file parsing and
 * creating book structs
 */

#include "core.h"
#include "avl.h"
#include "trie.h"

extern AvlNode *avl;
extern TrieNode *trie_title;
extern TrieNode *trie_name;

/* 
 * Chop trailing newline from a string inplace.
 * We're wasting a char this way, but it's faster
 */
static void chomp(char* s)
{
	int end = strlen(s) - 1;
	if (end >= 0 && s[end] == '\n') 
		s[end] = '\0';
}

/* create and return a book struct */
static Book* create_book(long given_id, char* name, short year, char* pub)
{
	Book *b = smalloc(sizeof(Book));
	b->id = given_id;
	strncpy(b->title, name, 256);
	strncpy(b->publisher, pub, 40);
	b->yearPublished = year;
	b->numberOfAuthors = 0;
	return b;
}

/* dynamically change (and returns) the authors array in a given book struct */
static Author* create_author(Book* b, char* f, char* l)
{
	Author *a;
	unsigned int index = b->numberOfAuthors;
	/* creating or reallocating the proper array */
	if (index == 0) {
		a = smalloc(sizeof(Author));
	} else {
		a = realloc(b->authors, (sizeof(Author)*(index+1)) );
		if (!a) fatal("while reallocating to add author");
	}
	/* now add the names and point back */
	strncpy(a[index].first, f, 56);
	strncpy(a[index].last,  l, 56);
	/* increment the counter */
	b->numberOfAuthors++;
	return a;
}

/* function to split into different authors and first and last names.
 * and assign them. Super simplified. Assuming first-name comes first */
void split_create_authors(Book* B, char* full)
{
	int count;
	char *next;
	char first_name[56];
	char last_name[56];

	count = 0;
	next = strtok(full, " ,");
	while (next != NULL) {
		/* first names are even numbers */
		if (count % 2 == 0)
			strncpy(first_name, next, 56);
		else {
			strncpy(last_name, next, 56);
			B->authors = create_author(B, first_name, last_name);
		}
		count++;
		next = strtok(NULL, " ,");
	}
	/* possibility of just one first name */
	if (count == 1) {
		strcpy(last_name, " ");
		B->authors = create_author(B, first_name, last_name);
	}
}

/* Linear search to find the array index of a book with a given id */
long find_index_by_id(long id)
{
	unsigned long i;

	for (i = 0; i < db.numberOfBooks; i++) {
		if (db.arr[i].id == id) {
			return i;
		}
	}
	/* else */
	return -1;
}

/* internal function to resize the db and add a book */
int add_book(Book* A)
{
	long size = (db.numberOfBooks + 1) * sizeof(Book);
	char *name;
	char *title;
	/* the new array */
	Book* D;

	/* make it bigger */
	D = realloc(db.arr, size);
	if (D) {
		db.arr = D;
		/* now add the book at the end */
		db.arr[db.numberOfBooks] = *A;
		db.numberOfBooks++;
		/* trees */
		title = db.arr[db.numberOfBooks].title;
		name  = db.arr[db.numberOfBooks].authors[0].last;

		avl  = avl_insert(&db.arr[db.numberOfBooks], avl);
		trie_title = trie_insert(title, &db.arr[db.numberOfBooks], trie_title);
		trie_name  = trie_insert(name,  &db.arr[db.numberOfBooks], trie_name);
		/* finally */
		sorted = 0;
		free(A);
		return 0;
	} else
		return -1;
}

/* internal function for general removing by index */
int remove_book(long index)
{
	Book *D;
	Book tmp;
	char *name;
	char *title;
	long size = (db.numberOfBooks - 1) * sizeof(Book);

	title = db.arr[index].title;
	name  = db.arr[index].authors[0].last;
	/* removing from trees */
	avl  = avl_delete(db.arr[index].id, avl);
	trie_title = trie_delete(title, trie_title);
	trie_name  = trie_delete(name,  trie_name);

	/* moving the interesting node at the end */
	tmp = db.arr[index];
	memmove(db.arr+index, db.arr+index+1,
			sizeof(Book)*(db.numberOfBooks-index-1));
	db.arr[db.numberOfBooks - 1] = tmp;

	/* first freeing any dynamic memory */
	free(db.arr[db.numberOfBooks - 1].authors);

	/* removing by reallocating by one Book size to free the coresponding
	 * memory of the interesting node */
	D = realloc(db.arr, size);
	if (D) {
		/* replace and reduce the sum */
		db.arr = D;
		db.numberOfBooks--;
		return 0;
	} else
		return -1;
}

/* load and initialize db from file first menu option function */
void init_db(const char *file)
{
	/* temp buffers and descriptors */
	FILE *fd;
	char given_id[11];
	char line[256+56+56+4+40];
	char book_name[256];
	char writer[56*2];
	char the_year[4];
	char pub_name[40];

	fd = fopen(file, "r");
	if (!fd)
		fatal("while opening data file");
	/* get the total number of books */
	if (!fgets(line, 8, fd))
		fatal("while reading data file");

	long n = atol(line);
	if (!n)
		fatal("no sum of books at the first line");
	/* allocate the main database */
	db.arr = smalloc(sizeof(Book)*n);
	db.numberOfBooks = 0;
	/* start parsing the file, delimiter is ';' */
	memset(line, 0, sizeof(line));
	printf("Loading file...\n");
	while (fgets(line, sizeof(Book), fd) != NULL) {

		/* erase the previous data */
		memset(given_id, 0, sizeof(given_id));
		memset(book_name, 0, sizeof(book_name));
		memset(writer, 0, sizeof(writer));
		memset(the_year, 0, sizeof(the_year));
		memset(pub_name, 0, sizeof(pub_name));

		/* scanf is like magic */
		sscanf(line, "\"%[0-9]\";\"%[0-9a-zA-Z.:!'?,)( ]\";\"%[0-9a-zA-Z.:',!?)( ]\";\"%[0-9]\";\"%[0-9a-zA-Z.:'!,?)( ]\"",
						given_id, book_name, writer, the_year, pub_name);

		/* the symbols in the scanf sequence are the only ones we accept to
		 * be in the strings and we don't want any weird stuff because they
		 * break the encoding, so we're dropping any books that apper to have
		 * no authors */
		if (strlen(writer) == 0) {
			memset(line, 0, sizeof(line));
			continue;
		}

		/* set defaults here */
		if (strlen(the_year) == 0)
			strcpy(the_year, "0000");

		/* check for duplicates */
		if (btraverse(atol(given_id), 1) != -1)
			continue;

		/* Done parsing. Creating book */
		Book *B;
		B = create_book(atol(given_id), book_name, atoi(the_year), pub_name);

		/* creating authors */
		split_create_authors(B, writer);

		/* put it in the actual database */
		db.arr[db.numberOfBooks] = *B;
		db.numberOfBooks++;

		/* clean */
		memset(line, 0, sizeof(line));
		/* we copy the data above, so we don't need that */
		free(B);
	}
	fclose(fd);
}

/* print the dynamic db back to the provided file (can handle stdout) */
void print_db(const char *file)
{
	FILE *fd;
	int i;
	int n;
	int j;
	int size;
	char *name;

	if (strcmp(file, "stdout") == 0){
		fd = stdout;
	}
	else {
		fd = fopen(file, "w");
		if (!fd) fatal("while opening data file for writing");
	}
	/* first we write the total number of books */
	fprintf(fd, "%lu\n", db.numberOfBooks);
	/* allocate memory for one and reallocate accordingly */
	size = sizeof(char) * 56 * 2;
	name = smalloc(size);
	/* iterate over the db and add to file */
	for (i=0; (unsigned long)i < db.numberOfBooks; i++) {
		n = db.arr[i].numberOfAuthors;
		if (n > 1) {
			/* reallocating here in case of multiple authors */
			name = realloc(name, size * n);
		}
		/* concatenate the first and last name of every author */
		for (j=1; j <= n; j++) {
			strcat(name, db.arr[i].authors[j-1].first);
			if (db.arr[i].authors[j-1].last[0] != '\0') {
				strcat(name, " ");
				strcat(name, db.arr[i].authors[j-1].last);
			}
			if (n > 1 && j < n)
				strcat(name, ",");
		}
		fprintf(fd, "\"%ld\";\"%s\";\"%s\";\"%d\";\"%s\"\n",
					db.arr[i].id,
					db.arr[i].title,
					name,
					db.arr[i].yearPublished,
					db.arr[i].publisher);
		memset(name, 0, sizeof(size * n));
	}
	free(name);
	if (fd != stdout)
		fclose(fd);
}

/* resize the db and add a book struct with user input
 * this lives here since it uses static db functions */
void user_add_book(void)
{
	Book* B;
	long id;
	int year;
	char title[257];
	char name[112];
	char pub[41];

	/* get user input */
	fprintf(stdout, "Title of the Book? ");
	clear_stream();

	if (! fgets(title, sizeof(title) - 1, stdin))
		/* checking for error */
		goto error;
	chomp(title);

	printf("Author? ");
	if (! fgets(name, sizeof(name) - 1, stdin))
		goto error;
	chomp(name);

	printf("ID (10 numbers)? ");
	if (scanf("%ld", &id) == 0)
		goto error;

	/* clearing stream in another way due to scanf */
	while(getchar() != '\n');

	printf("When was it published? ");
	if (scanf("%d", &year) == 0)
		goto error;
	while(getchar() != '\n');

	printf("Publisher company? ");
	if (! fgets(pub, sizeof(pub) - 1, stdin))
		goto error;
	chomp(pub);

	/* create and call function to add it */
	B = create_book(id, title, year, pub);
	split_create_authors(B, name);

	if (!add_book(B)) {
		printf("Book node added to database [id: %ld]\n", B->id);
		return;
	}

error:
	printf("Book creation aborted\n");
	return;
}
