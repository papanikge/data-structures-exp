# Data structures project makefile
# George Papanikolaou
# absolutely no warranty
#
# $? is for the files on the right of the :
# $@ is for the target name

CC=gcc
WARN=-Wall -Wextra
OPT=-O0
FLAGS=-g

# default target that compiles and runs
all: books
	@./program

# the compilation target
books: books.c books.h
	$(CC) $(WARN) $(OPT) $(FLAGS) books.c -o program

clean:
	-@rm -f program a.out

.PHONY: clean
