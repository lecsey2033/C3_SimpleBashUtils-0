#ifndef SRC_GREP_S21_GREP_H_
#define SRC_GREP_S21_GREP_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <regex.h>
#include <getopt.h>
#include <errno.h>

#define BUFF 2048 * 4

typedef struct {
    int e;
    int i;
    int v;
    int c;
    int l;
    int n;
    int h;
    int s;
    int f;
    int o;
} flags;

// main function
int parser(int argc, char *argv[], flags *flag, char *pattern, char *filename);
void grep_program(int argc, char *argv[], char *buff, char *filename, flags *flag);
void grep_print(char *pattern, char *filename, flags *flag);
void print_text(FILE *f, regex_t reg, char *filename, flags *flag);

// helper function
void clear_struct(flags *flag);
void pattern_from_file(char *pattern, char *filename);

#endif  // SRC_GREP_S21_GREP_H_
