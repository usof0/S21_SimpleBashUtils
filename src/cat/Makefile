cc = gcc
flags = -Wall -Werror -Wextra -std=c11
clang_path = ../../materials/build/scripts/

.PHONY: all clean rebuild

all: clean s21_cat

s21_cat:
	$(cc) $(flags) s21_cat.c -o s21_cat
	
clean:
	rm -rf s21_cat
	rm -rf s21_cat *.out
	rm -rf .clang-format

testing:
	make clean
	make all
	sh tests.sh

rebuild: clean all

clang_replace:
	cp $(clang_path).clang-format ./
	clang-format -i *.c *.h