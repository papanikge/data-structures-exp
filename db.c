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

/* Count the specified character inside the given string */
static unsigned int count_char(char* str, const char c)
{
	int i;
	unsigned int count = 0;
	for (i = 0; str[i] != '\0'; i++)
		if (str[i] == c) count++;
	return count;
}

/* Split a char array to space, so we can get first and last name */
static void split_names(char* whole, char* first, char* last)
{
	unsigned int next_space = strcspn(whole, " ");
	strncpy(first, whole, next_space);
	strncpy(last , whole + next_space + 1, strlen(whole));
	/* now there is probably a space prefixed */
	if (last[0] == ' ')
		memmove(last, last + 1, strlen(last));
}

/* create and return a book struct */
static Book* create_book(char* name, short year, char* pub)
{
	Book *b = smalloc(sizeof(Book));
	b->id = db.numberOfBooks;
	/* the rest */
	strcpy(b->title, name);
	strcpy(b->publisher, pub);
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

/* load and initialize db from file first menu option function */
void init_db(const char *file)
{
	/* temp buffers and descriptors */
	FILE *fd;
	unsigned int i,s;
	unsigned int next_comma;
	unsigned int next_space;
	char line[256+56+56+4+40];
	char book_name[256];
	char writer[56*2];
	char first_name[56];
	char last_name[56];
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
		memset(book_name, 0, sizeof(book_name));
		memset(writer, 0, sizeof(writer));
		memset(first_name, 0, sizeof(first_name));
		memset(last_name, 0, sizeof(last_name));
		memset(the_year, 0, sizeof(the_year));
		memset(pub_name, 0, sizeof(pub_name));

		/* scanf is like magic */
		sscanf(line, "\"%[0-9a-zA-Z.:!'?,)( ]\";\"%[0-9a-zA-Z.:',!?)( ]\";\"%[0-9]\";\"%[0-9a-zA-Z.:'!,?)( ]\"",
						book_name, writer, the_year, pub_name);

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

		/* done. creating... */
		Book *B;
		B = create_book(book_name, atoi(the_year), pub_name);

		/* splitting into different authors and first and last names */
		next_comma = strcspn(writer, ",");
		if (next_comma == strlen(writer)) {
			/* this means there is no comma, so only one author */
			split_names(writer, first_name, last_name);

			/* add him */
			B->authors = create_author(B, first_name, last_name);
		} else {
			while (count_char(writer, ',') != 0) {
				next_space = strcspn(writer, " ");
				/* clear */
				memset(first_name, 0, sizeof(first_name));
				memset(last_name, 0, sizeof(last_name));

				if (next_space > next_comma) {
					/* unlikely but the comma can come before a space */
					strncpy(first_name, writer, next_comma);
					strcpy(last_name, " ");
				} else {
					/* not using split_names() in here due to multiples */
					strncpy(first_name, writer, next_space);
					strncpy(last_name, writer + next_space + 1, next_comma - next_space - 1);
				}

				/* add him to the db... */
				B->authors = create_author(B, first_name, last_name);

				/* we need to remove everything before the working comma
				 * so the next iteration would work.
				 * First find the lenght of the interesting area */
				s = 0;
				for (i = 0; writer[i+next_comma+1] != '\0'; i++)
					s++;
				if (s == 0)
					break;
				/* and finally... */
				memmove(writer, writer + next_comma + 1, s);
				memset(writer + s, 0, sizeof(writer)-s);
				next_comma = strcspn(writer, ",");
			}
		}

		/* put it in the actual database */
		db.arr[B->id] = *B;
		db.numberOfBooks++;
		memset(line, 0, sizeof(line));
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

	if (!strcmp(file, "stdout")){
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
	memset(name, 0, size);
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
		fprintf(fd, "\"%s\";\"%s\";\"%d\";\"%s\"\n",
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

/* resize the db and add a book struct with user input */
void user_add_book(void)
{
	Book* B;
	Book* D;
	int tmp;
	char title[257];
	char name[112];
	char fst[112];
	char snd[112];
	char year[6];
	char pub[41];
	long size = (db.numberOfBooks + 1) * sizeof(Book);

	/* get user input */
	fprintf(stdout, "Title of the Book? ");
	/* sometimes there is a trailing newline around and we need to consume it*/
	tmp = fgetc(stdin);
	if (tmp != '\n')
		ungetc(tmp, stdin);

	if (! fgets(title, sizeof(title) - 1, stdin))
		/* checking for error */
		goto error;
	chomp(title);

	printf("Author? ");
	if (! fgets(name, sizeof(name) - 1, stdin))
		goto error;
	chomp(name);

	printf("When was it published? ");
	if (! fgets(year, sizeof(year), stdin))
		goto error;
	chomp(year);
	/* XXX: maybe assert that this is 4 numbers */
	printf("Publisher company? ");
	if (! fgets(pub, sizeof(pub) - 1, stdin))
		goto error;
	chomp(pub);

	/* make it bigger */
	D = realloc(db.arr, size);
	if (!D) fatal("while reallocating the db to add a struct");
	db.arr = D;

	B = create_book(title, atoi(year), pub);

	/* take care of the authors */
	split_names(name, fst, snd);
	B->authors = create_author(B, fst, snd);

	/* now add the book at the end */
	db.arr[db.numberOfBooks] = *B;
	db.numberOfBooks++;
	printf("Book node added to database [id: %ld]\n", B->id);
	return;

error:
	printf("Book creation aborted\n");
	return;
}

void user_remove_book(void)
{
	Book *D;
	Book tmp;
	char title[256];
	long rid;
	long size = (db.numberOfBooks - 1) * sizeof(Book);

	printf("ID of the book to remove? ");
	scanf("%ld", &rid);
	if (rid < 0 || (unsigned long)rid >= db.numberOfBooks) {
		printf("Book removal aborted\n");
		return;
	}

	/* keep the title for later */
	strcpy(title, db.arr[rid].title);

	/* moving the interesting node at the end */
	tmp = db.arr[rid];
	memmove(db.arr+rid, db.arr+rid+1, db.numberOfBooks-rid-1);
	db.arr[db.numberOfBooks] = tmp;

	/* first freeing any dynamic memory */
	free(db.arr[db.numberOfBooks].authors);

	/* removing by reallocating by one Book size to free the coresponding
	 * memory of the interesting node */
	D = realloc(db.arr, size);
	if (!D) fatal("while reallocating the db to remove a struct");
	db.arr = D;

	printf("\"%s\" removed from database\n", title);
	return;
}
