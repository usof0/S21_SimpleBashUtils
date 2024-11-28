#ifndef S21_GREP_H
#define S21_GREP_H

#define line_length 1024
#define usage_error                                                           \
  "usage: ./s21_grep [-abcdDEFGHhIiJLlMmnOopqRSsUVvwXxZz] [-A num] [-B num] " \
  "[-C[num]]\n\t[-e pattern] [-f file] [--binary-files=value] "               \
  "[--color=when]\n\t[--context[=num]] [--directories=action] "               \
  "[--label] [--line-buffered]\n\t[--null] [pattern] [file ...]"

struct options {
  int e[16], i, v, c, l, n, h, s, f, o;
};

void check_options(int argc, char **argv);
void add_option(char *flag, int *arg_index, struct options *cur_flags,
                char **argv);
void flag_v(int index, char **argv);
void grep_files(char **argv);
void grep_templates(int file_number, char *line, int line_number, char **argv,
                    int *counter, int *last_line);
void output(int file_number, char *line, int line_number, char **argv);
void flags_init(struct options *x);
void output_h(char *name, int number);

#endif