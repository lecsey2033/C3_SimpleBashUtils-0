#include "./s21_grep.h"

int main(int argc, char *argv[]) {
    int result;
    flags flag;

    clear_struct(&flag);
    char pattern[BUFF] = {0};
    char filename[BUFF] = {0};
    result = parser(argc, argv, &flag, pattern, filename);
    if (argc > 2 && result != -1) {
        grep_program(argc, argv, pattern, filename, &flag);
    } else {
      printf("usage: grep [-eivclnhsfo]\n");
      printf("\t[-e pattern] [-f file]\n");
      printf("\t[pattern] [file ...]\n");
    }

    return 0;
}

void clear_struct(flags *flag) {
    flag->c = 0;
    flag->e = 0;
    flag->f = 0;
    flag->h = 0;
    flag->i = 0;
    flag->l = 0;
    flag->n = 0;
    flag->o = 0;
    flag->s = 0;
    flag->v = 0;
}

int parser(int argc, char *argv[], flags *flag, char *pattern, char *filename) {
    int opt = 0, result = 0;
    while ((opt = getopt(argc, argv, "e:ivclnhsf:o")) != -1) {
        switch (opt) {
            case 'e': {
                flag->e = 1;
                strcat(pattern, optarg);
                strcat(pattern, "|");
                break;
                }
            case 'i': { flag->i = 1; break; }
            case 'v': { flag->v = 1; break; }
            case 'c': { flag->c = 1; break; }
            case 'l': { flag->l = 1; break; }
            case 'n': { flag->n = 1; break; }
            case 'h': { flag->h = 1; break; }
            case 's': { flag->s = 1; break; }
            case 'o': { flag->o = 1; break; }
            case 'f': { flag->f = 1; snprintf(filename, BUFF, optarg); break; }
            default: {
                printf("usage: grep [-eivclnhsfo]\n");
                result = -1;
                break;
            }
        }
    }
    return result;
}

void grep_program(int argc, char *argv[], char *pattern, char *filename, flags *flag) {
    char tmp_pattern[BUFF] = {0};

    if (!flag->f && !flag->e) {
        snprintf(tmp_pattern, BUFF, argv[optind++]);
    }
    if (!flag->f && flag->e) {
        int i = strlen(pattern);
        pattern[i-1] = '\0';
        snprintf(tmp_pattern, BUFF, pattern);
    }
    if (flag->f && flag->e) {
        snprintf(tmp_pattern, BUFF, pattern);
        pattern_from_file(tmp_pattern, filename);
    }
    int C_File = 0;
    if (argc - optind > 1) {
        C_File = 1;
    }
    for (int i = optind; i < argc; i++) {
        if (C_File && !flag->h && !flag->l) {
            printf("%s:", argv[i]);
        }
        grep_print(tmp_pattern, argv[i], flag);
    }
}

void pattern_from_file(char *pattern, char *filename) {
    int i = strlen(pattern);
    FILE *file = fopen(filename, "r");

    if (file != NULL) {
        int symbol;
        while ((symbol = fgetc(file)) != EOF) {
            if (symbol != 10 && symbol != 13) {
                pattern[i++] = symbol;
            }
            if (symbol == 10 || symbol == 13) {
                pattern[i++] = '|';
            }
        }
    } else {
        printf("%s\n", strerror(errno));
        i = -1;
    }
    if (pattern[i - 1] == '|') {
        pattern[i - 1] = '\0';
    }
    fclose(file);
}

void grep_print(char *pattern, char *filename, flags *flag) {
    int cflags = (flag->i) ? REG_ICASE | REG_EXTENDED : REG_EXTENDED;
    regex_t reg;
    FILE *file = fopen(filename, "r");

    if (file != NULL) {
        regcomp(&reg, pattern, cflags);
        print_text(file, reg, filename, flag);
        regfree(&reg);
        fclose(file);
    } else if (!flag->s) { printf("%s\n", strerror(errno)); }
}

void print_text(FILE *file, regex_t reg, char *filename, flags *p) {
    char text[BUFF] = {0};
    regmatch_t pmatch[1];
    int line_number = 1, line_matches = 0;

    while (fgets(text, BUFF - 1, file) != NULL) {
        int match = 0;
        int result = regexec(&reg, text, 1, pmatch, 0);

        if (result == 0 && !p->v) { match = 1; }
        if (result == REG_NOMATCH && p->v) { match = 1; }
        if (strchr(text, '\n') == NULL) { strcat(text, "\n"); }
        if (match && !p->l && !p->c && p->n) { printf("%d:", line_number); }
        if ((match && !p->l && !p->c && !p->o) || (match && p->v && p->o)) { printf("%s", text); }
        if (match && p->o && !p->v && !p->c && !p->l) {
            for (int i = pmatch[0].rm_so; i < pmatch[0].rm_eo; i++) {
               printf("%c", text[i]);
            }
            printf("\n");
        }
        line_matches += match;
        line_number += 1;
    }
    if (p->c && !p->l) { printf("%d\n", line_matches); }
    if (p->c && p->l) {printf("1\n"); }
    if (p->l && line_matches > 0) { printf("%s\n", filename); }
}
