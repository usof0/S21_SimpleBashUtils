#ifndef S21_CAT_H
#define S21_CAT_H
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct options {
  bool b;
  bool e;
  bool n;
  bool s;
  bool t;
  bool v;
};

void check_options(int argc, char **argv);
void add_option(char *flag, int i, struct options *cur_flags);
void cat_file(const char *file_name);
void flags_init(struct options *x);

#endif