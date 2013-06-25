# Data structures project makefile
# George Papanikolaou
# absolutely no warranty
#
# $? is for the files on the right of the :
# $@ is for the target name
#
# tags target requires ctags tool

CC=gcc
WARN=-Wall -Wextra
OPT=-O3
FLAGS=-g
HEADER=core.h

# default target that compiles and runs
all: core db search avl trie
	$(CC) core.o db.o sort-search.o avl.o trie.o -o program
	rm *.o
	@./program

# the compilation target
core: core.c $(HEADER)
	$(CC) $(WARN) $(OPT) $(FLAGS) -c core.c

db: db.c $(HEADER)
	$(CC) $(WARN) $(OPT) $(FLAGS) -c db.c

search: sort-search.c $(HEADER)
	$(CC) $(WARN) $(OPT) $(FLAGS) -c sort-search.c

avl: avl.c avl.h $(HEADER)
	$(CC) $(WARN) $(OPT) $(FLAGS) -c avl.c

trie: trie.c trie.h $(HEADER)
	$(CC) $(WARN) $(OPT) $(FLAGS) -c trie.c

clean:
	-@rm -f program base a.out

tags:
	ctags -R .

.PHONY: clean tags
