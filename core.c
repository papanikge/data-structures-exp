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
/* for the individual ids of book structs */
long idSum;

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
static int get_option(void)
{
	int in;
	do {
		printf("\nOption? [1..9] ");
		in = getchar();
	} while (in < 48 || in > 57);
	/* damn you ascii */
	return (in - 48);
}

int main(int argc, const char **argv)
{
	unsigned int opt;
	/* handle data base file name */
	char filename[50];
	if (argc >= 2)
		strcpy(filename, argv[1]);
	else
		strcpy(filename, "datafile.db");

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
				free(db.arr);
				init_db(filename);
				break;
			case 2:
				save_db(filename);
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
	save_db(filename);
	return 0;
}
