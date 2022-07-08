#ifndef SRC_CAT_S21_CAT_H_
#define SRC_CAT_S21_CAT_H_

#include <stdio.h>
#include <getopt.h>
#include <errno.h>
#include <string.h>

typedef struct {
    int b;
    int e;
    int n;
    int s;
    int t;
    int v;
} flag;

void clear_struct(flag* opt);
int parser(int argc, char* argv[], flag* opt);
void print_stdin();
void reader(char *argv[], flag *opt);


#endif  // SRC_CAT_S21_CAT_H_
