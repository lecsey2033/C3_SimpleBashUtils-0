.PHONY: all clean test check rebuild s21_cat
CC=gcc
CFLAGS=-Wall -Werror -Wextra -std=c11 -D_GNU_SOURCE

all: clean s21_cat

s21_cat:
	${CC} ${CFLAGS} s21_cat.c -o s21_cat

clean:
	@rm -rf s21_cat.d* s21_cat s21*o *cfg

check: s21_cat
	cp ../../materials/linters/CPPLINT.cfg ./
	cppcheck --enable=all --suppress=missingIncludeSystem *.c *.h
	python3 ../../materials/linters/cpplint.py --extensions=c *.c *.h
	rm -rf CPPLINT.cfg

test: s21_cat
	bash t_cat.sh
	rm -rf log.txt

rebuild: clean all