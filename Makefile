# Data structures project makefile
# George Papanikolaou
# absolutely no warranty

CC=gcc
WARN=-Wall -Wextra
OPT=-O0
FLAGS=-g

# all compiles and runs
all: base
	@./base

# the compilation target
base: base.c
	$(CC) $(WARN) $(OPT) $(FLAGS) $? -o $@

clean:
	-@rm base a.out datafile.db || true

.PHONY: clean
