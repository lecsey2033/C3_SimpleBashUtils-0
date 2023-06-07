#include "s21_cat.h"

int main(int argc, char* argv[]) {
    if (argc > 1) {
        flag opt;
        clear_struct(&opt);
        opterr = 0;
        int result = parser(argc, argv, &opt);
        if (opt.b) {
            opt.n = 0;
            }
        if (result != -1) {
            while (optind < argc) {
                reader(argv, &opt);
                optind++;
            }
        }
    } else {
        print_stdin();
    }
    return 0;
}

void clear_struct(flag* opt) {
    opt->b = 0;
    opt->e = 0;
    opt->n = 0;
    opt->s = 0;
    opt->t = 0;
    opt->v = 0;
}

int parser(int argc, char* argv[], flag* opt) {
    int result = 0;
    int option = 0;
    struct option GNU_option[] = {
        {"number-nonblank", 0, NULL, 'b'},
        {"number", 0, NULL, 'n'},
        {"squeeze-blank", 0, NULL, 's'},
        {NULL, 0, NULL, 0}
    };

    while ((option = getopt_long(argc, argv, "bevEnstT", GNU_option, NULL)) != -1) {
        switch (option) {
            case 'b': { opt->b = 1; break; }
            case 'e': { opt->e = 1; opt->v = 1; break; }
            case 'E': { opt->e = 1; break; }
            case 'n': { opt->n = 1; break; }
            case 's': { opt->s = 1; break; }
            case 't': { opt->t = 1; opt->v = 1; break; }
            case 'T': { opt->t = 1; break; }
            case 'v': { opt->v = 1; break; }
            default: {
                printf("s21_cat: illegal option -- %c\n", argv[1][1]);
                printf("usage: cat [-benstuv] [file ...]\n");
                result = -1;
                break;
            }
        }
    }
    return result;
}

void reader(char *argv[], flag *opt) {
    FILE* file = NULL;
    file = fopen(argv[optind], "r");
    if (file == NULL) {
        printf("s21_cat: %s: %s\n", argv[optind], strerror(errno));
    } else {
        int i = 1;
        int c;
        int previous_symbol = '\n', counter = 0;
        while ((c = fgetc(file)) != EOF) {
            if (c == '\n') {
                counter++;
            } else {
                counter = 0;
            }
            if (c == '\n' && (!opt->s || counter < 3)) {
                if (opt->n && previous_symbol == '\n') {
                    printf("%6d\t", i++);
                    }
                if (opt->e) {
                    printf("$");
                    }
                printf("%c", c);
            }
            if (c != '\n') {
                if ((previous_symbol == '\n') && (opt->n || opt->b)) {
                    printf("%6d\t", i++);
                }
                if (c < 32 && c != 9 && c != 10 && opt->v) {
                    printf("^%c", c + 64);
                } else if (c > 127 && c < 160 && opt->v) {
                    printf("M-^%c", c - 64);
                } else if (c == 127 && opt->v) {
                    printf("^%c", c - 64);
                } else if (c == '\t' && opt->t) {
                    printf("^I");
                } else { printf("%c", c); }
            }
            previous_symbol = c;
        }
        fclose(file);
    }
}

void  print_stdin() {
    int c = 0;
    while ((c = getchar()) != EOF)
        printf("%c", c);
}
