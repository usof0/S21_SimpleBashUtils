#include "s21_cat.h"

struct options flags;

int files[16];
int n = 0;

int main(int argc, char **argv) {
  flags_init(&flags);
  check_options(argc, argv);
  for (int i = 0; i < 16; i++) {
    if (files[i] && argv[files[i]]) {
      cat_file(argv[files[i]]);
    }
  }

  return 0;
}

void check_options(int argc, char **argv) {
  struct options cur_flags;
  flags_init(&cur_flags);
  for (int i = 1; i < argc; i++) {
    if (argv[i][0] == '-') {
      add_option(argv[i], i, &cur_flags);
    } else {
      files[n] = i;
      n++;
    }
  }
}

void add_option(char *flag, int file_index, struct options *cur_flags) {
  if (flag[1] == '-') {
    if (strcmp(flag, "--number-nonblank") == 0) {
      flags.b = 1;
    } else if (strcmp(flag, "--number") == 0) {
      flags.n = 1;
    } else if (strcmp(flag, "--squeeze-blank") == 0) {
      flags.s = 1;
    } else {
      files[n] = file_index;
      n++;
    }
  } else if (files[0] > 0) {
    files[n] = file_index;
    n++;
  } else {
    int i = 1;
    while (flag[i] != '\0' && flag[i] != '\n') {
      if (flag[i] == 'b')
        cur_flags->b = 1;
      else if (flag[i] == 'e')
        cur_flags->e = 1;
      else if (flag[i] == 'E') {
        cur_flags->e = 1;
        cur_flags->v = 1;
      } else if (flag[i] == 'n')
        cur_flags->n = 1;
      else if (flag[i] == 's')
        cur_flags->s = 1;
      else if (flag[i] == 't')
        cur_flags->t = 1;
      else if (flag[i] == 'T') {
        cur_flags->t = 1;
        cur_flags->v = 1;
      } else if (flag[i] == 'v')
        cur_flags->v = 1;
      else {
        files[n] = file_index;
        n++;
        cur_flags->b = 0;
        cur_flags->e = 0;
        cur_flags->n = 0;
        cur_flags->s = 0;
        cur_flags->t = 0;
        cur_flags->v = 0;
        break;
      }
      i++;
    }
    flags.b = cur_flags->b;
    flags.e = cur_flags->e;
    flags.n = cur_flags->n;
    flags.s = cur_flags->s;
    flags.t = cur_flags->t;
    flags.v = cur_flags->v;
  }
}

void cat_file(const char *file_name) {
  FILE *file = NULL;
  file = fopen(file_name, "r");
  if (file == NULL) {
    perror("Error opening file");
  } else {
    int line_number = 1;
    int prev_blank = 1;
    char c = '\0';
    while ((c = fgetc(file)) != EOF) {
      if (flags.s && prev_blank == 2 && c == '\n') {
        continue;
      }
      if (prev_blank > 0) {
        if (flags.b) {
          if (c != '\n')
            printf("%6d\t", line_number++);
          else if (flags.e)
            printf("      \t");
        } else if (flags.n) {
          printf("%6d\t", line_number++);
        }
      }
      if (flags.e) {
        if (c != '\n') {
          if (c >= 0 && c <= 31 && c != 9) {
            putchar('^');
            c = c + 64;
          } else if (c >= 127) {
            putchar('^');
            c = c - 64;
          }
        } else
          putchar('$');
      }
      if (flags.v && (c < 32 || c > 126) && c != '\n' && c != '\t') {
        putchar('^');
        if (c < 32)
          c = c + 64;
        else
          c = c - 64;
      }
      if (flags.t) {
        if (c >= 0 && c <= 31 && c != 10) {
          putchar('^');
          c = c + 64;
        } else if (c >= 127) {
          putchar('^');
          c = c - 64;
        }
      }
      putchar(c);
      if (c == '\n')
        prev_blank++;
      else
        prev_blank = 0;
    }
    fclose(file);
  }
}

void flags_init(struct options *x) {
  x->b = 0;
  x->e = 0;
  x->n = 0;
  x->s = 0;
  x->t = 0;
  x->v = 0;
}