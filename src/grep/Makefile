.PHONY: all clean test check rebuild s21_grep
CC=gcc
CFLAGS=-Wall -Werror -Wextra -std=c11 -D_GNU_SOURCE

all: clean s21_grep

s21_grep:
	${CC} ${CFLAGS} s21_grep.c -o s21_grep

clean:
	@rm -rf s21_grep.d* s21_grep s21*o *cfg

check: s21_grep
	cp ../../materials/linters/CPPLINT.cfg ./
	cppcheck --enable=all --suppress=missingIncludeSystem *.c *.h
	python3 ../../materials/linters/cpplint.py --extensions=c *.c *.h
	rm -rf CPPLINT.cfg
	CK_FORK=no leaks -atExit -- ./s21_grep

test: s21_grep
	bash test.sh
	rm -rf log.txt

rebuild: clean all