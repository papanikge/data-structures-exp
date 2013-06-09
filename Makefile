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
all: core db
	$(CC) core.o db.o -o program
	rm *.o
	@./program

# the compilation target
core: core.c core.h
	$(CC) $(WARN) $(OPT) $(FLAGS) -c core.c

db: db.c
	$(CC) $(WARN) $(OPT) $(FLAGS) -c db.c

clean:
	-@rm -f program base a.out

tags:
	ctags -R .

.PHONY: clean tags
