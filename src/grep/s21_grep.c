#include "s21_grep.h"

#include <regex.h>
#include <stdio.h>
#include <string.h>

struct options flags;

int files[16], templates[16], n = 0, t = 0, x = 0;
char templates2[8][256];
int x2 = 0, f = 0, v = 0;

int main(int argc, char **argv) {
  flags_init(&flags);
  for (int i = 0; i < 8; i++) templates2[i][0] = '\0';
  for (int i = 0; i < 16; i++) files[i] = 0;
  for (int i = 0; i < 16; i++) templates[i] = 0;
  check_options(argc, argv);
  for (int i = n; i < 16; i++) files[i] = 0;
  for (int i = t; i < 16; i++) templates[i] = 0;
  grep_files(argv);
  if (flags.c || flags.l) printf("\n");

  return 0;
}

void check_options(int argc, char **argv) {
  struct options cur_flags;
  flags_init(&cur_flags);
  for (int i = 1; i < argc; i++) {
    if (flags.e[0] != 0 && flags.e[x - 1] == i - 1) {
      templates[t] = i;
      t++;
    } else if (argv[i][0] == '-') {
      add_option(argv[i], &i, &cur_flags, argv);
    } else if (f && f == i - 1) {
      FILE *file = fopen(argv[i], "r");
      if (file != NULL) {
        char line[line_length] = "\0";
        while (fgets(line, line_length, file)) {
          strcpy(templates2[x2], line);
          x2++;
        }
      }
      fclose(file);
    } else if (t == 0 && x2 == 0) {
      templates[t] = i;
      t++;
    } else {
      files[n] = i;
      n++;
    }
  }
}

void add_option(char *flag, int *arg_index, struct options *cur_flags,
                char **argv) {
  int i = 1;
  while (flag[i] != '\0' && flag[i] != '\n') {
    if (flag[i] == 'e') {
      cur_flags->e[0] = *arg_index;
      if (flag[i + 1] != ' ' && flag[i + 1] != '\n') {
        strcpy(templates2[x2], &flag[i + 1]);
        x2++;
        break;
      }
    } else if (flag[i] == 'i') {
    } else if (flag[i] == 'v') {
      cur_flags->v = 1;
      v = *arg_index;
    } else if (flag[i] == 'c') {
      cur_flags->c = 1;
    } else if (flag[i] == 'l') {
      cur_flags->l = 1;
    } else if (flag[i] == 'n') {
      cur_flags->n = 1;
    } else if (flag[i] == 'h') {
      cur_flags->h = 1;
    } else if (flag[i] == 's') {
      cur_flags->s = 1;
    } else if (flag[i] == 'f') {
      cur_flags->f = 1;
      f = *arg_index;
      *arg_index += 1;
      flag_v(*arg_index, argv);
    } else if (flag[i] == 'o') {
      cur_flags->o = 1;
    } else {
      printf("%s", usage_error);
      cur_flags->e[0] = 0;
      cur_flags->i = 0;
      cur_flags->v = 0;
      cur_flags->c = 0;
      cur_flags->l = 0;
      cur_flags->n = 0;
      cur_flags->h = 0;
      cur_flags->s = 0;
      cur_flags->f = 0;
      cur_flags->o = 0;
    }
    i++;
  }
  if (cur_flags->i) flags.i = cur_flags->i;
  if (cur_flags->v) flags.v = cur_flags->v;
  if (cur_flags->c) flags.c = cur_flags->c;
  if (cur_flags->l) flags.l = cur_flags->l;
  if (cur_flags->n) flags.n = cur_flags->n;
  if (cur_flags->h) flags.h = cur_flags->h;
  if (cur_flags->s) flags.s = cur_flags->s;
  if (cur_flags->f) flags.f = cur_flags->f;
  if (cur_flags->o) flags.o = cur_flags->o;
  if (cur_flags->e[0] != 0) {
    flags.e[x] = cur_flags->e[0];
    x++;
    cur_flags->e[0] = 0;
  }
}

void flag_v(int index, char **argv) {
  FILE *file = NULL;
  file = fopen(argv[index], "r");
  if (file != NULL) {
    char c[255] = "\0";
    while (fgets(c, 255, file)) {
      strcpy(templates2[x2], c);
      x2++;
    }
    fclose(file);
  }
}

void grep_files(char **argv) {
  int l_count = 0;
  for (int i = 0; i < n; i++) {
    FILE *file = fopen(argv[files[i]], "r");
    int counter = 0, last_line = 0;
    if (file == NULL) {
      if (!flags.s)
        printf("/.s21_grep: %s: No such file or directory\n", argv[files[i]]);
    } else {
      char line[line_length] = "\0";
      int j = 1;

      while (fgets(line, line_length, file)) {
        grep_templates(i, line, j, argv, &counter, &last_line);
        j++;
      }
      if (j == last_line + 1 && !flags.l && !flags.c) printf("\n");
      fclose(file);
    }
    if (flags.c && flags.l) {
      int pos = 0;
      if (counter > 0) pos = 1;
      if (i > 0 && pos == 0) printf("\n");
      if (n == 1)
        printf("%d", pos);
      else {
        if (!flags.l) printf("%s:", argv[files[i]]);
        if (i != 0 && (t != 0 || x2 != 0) && pos == 1)
          printf("\n");  //&& !flags.l && !flags.h
        if (!flags.h) printf("%s:", argv[files[i]]);
        printf("%d", pos);
      }
      if (pos == 1 && flags.l) printf("\n");  ////////
    } else if (flags.c) {
      if (i > 0) printf("\n");
      if (n == 1)
        printf("%d", counter);
      else
        output_h(argv[files[i]], counter);
    }
    if (flags.l && counter > 0) {
      if (l_count > 0 && !flags.c) printf("\n");
      printf("%s", argv[files[i]]);
      l_count++;
    }
  }
}

void grep_templates(int file_number, char *line, int line_number, char **argv,
                    int *counter, int *last_line) {
  int matching = 0;
  regex_t regex;
  for (int i = 0; i < t; i++) {
    int regt;
    if (flags.i)
      regt = regcomp(&regex, argv[templates[i]], REG_ICASE);
    else
      regt = regcomp(&regex, argv[templates[i]], 0);
    if (!regt) {
      regt = regexec(&regex, line, 0, NULL, 0);
      if (!regt) {
        matching++;
      }
    }
    regfree(&regex);
  }
  for (int i = 0; i < x2; i++) {
    int regt;
    if (flags.i)
      regt = regcomp(&regex, templates2[i], REG_ICASE);
    else
      regt = regcomp(&regex, templates2[i], 0);
    if (!regt) {
      regt = regexec(&regex, line, 0, NULL, 0);
      if (!regt) {
        matching++;
      }
    }
    regfree(&regex);
  }
  if (matching != 0 && !flags.v) {
    if (!flags.c && !flags.l) output(file_number, line, line_number, argv);
    *counter += 1;
    *last_line = line_number;
  } else if (matching == 0 && flags.v) {
    if (!flags.c && !flags.l) output(file_number, line, line_number, argv);
    *counter += 1;
    *last_line = line_number;
  }
}

void output(int file_number, char *line, int line_number, char **argv) {
  if (n > 1 && flags.h == 0) printf("%s:", argv[files[file_number]]);
  if (flags.n) printf("%d:", line_number);
  printf("%s", line);
}

void flags_init(struct options *x) {
  for (int i = 0; i < 16; i++) {
    x->e[i] = 0;
  }
  x->i = 0;
  x->v = 0;
  x->c = 0;
  x->l = 0;
  x->n = 0;
  x->h = 0;
  x->s = 0;
  x->f = 0;
  x->o = 0;
}

void output_h(char *name, int number) {
  if (flags.h)
    printf("%d", number);
  else
    printf("%s:%d", name, number);
}