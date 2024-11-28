cc = gcc
flags = -Wall -Werror -Wextra -std=c11
clang_path = ../../materials/build/scripts/

.PHONY: all clean rebuild

all: clean s21_grep

s21_grep:
	$(cc) $(flags) s21_grep.c -o s21_grep
	
clean:
	rm -rf s21_grep
	rm -rf s21_grep *.out
	rm -rf .clang-format

testing:
	make clean
	make all
	sh tests.sh

rebuild: clean all

clang_replace:
	cp $(clang_path).clang-format ./
	clang-format -i *.c *.h