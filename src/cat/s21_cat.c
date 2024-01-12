#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct s_avi_flags {
  char flag;               // флаг
  char *equivalent_flags;  // эквивалентные простые флаги
};

void append_flag(char *flags, char flag) {
  if (strchr(flags, flag) == NULL) {
    char temp[2] = "-";
    temp[0] = flag;
    strcat(flags, temp);
  }
}

void append_flags(char *flags, char flag) {
  struct s_avi_flags avi_flags[8] = {{'b', "b"},  {'E', "E"}, {'e', "Ev"},
                                     {'n', "n"},  {'s', "s"}, {'T', "T"},
                                     {'t', "Tv"}, {'v', "v"}};

  for (int i = 0; i < 8; i++) {
    if (avi_flags[i].flag == flag) {
      for (size_t j = 0; j < strlen(avi_flags[i].equivalent_flags); j++) {
        append_flag(flags, avi_flags[i].equivalent_flags[j]);
      }
      break;
    }
  }
}

void print_symb(int c, int *prev, char *flags, int *index,
                bool *eline_printed) {
  if (!(strchr(flags, 's') != NULL && *prev == '\n' && c == '\n' &&
        *eline_printed)) {
    if (*prev == '\n' && c == '\n')
      *eline_printed = 1;
    else
      *eline_printed = 0;

    if (((strchr(flags, 'n') != NULL && strchr(flags, 'b') == NULL) ||
         (strchr(flags, 'b') != NULL && c != '\n')) &&
        *prev == '\n') {
      *index += 1;
      printf("%6d\t", *index);
    }

    if (strchr(flags, 'E') != NULL && c == '\n') {
      if (*prev != '$') {
        printf("$");
      }
    }
    if (strchr(flags, 'T') != NULL && c == '\t') {
      printf("^");
      c = '\t' + 64;
    }
    if (strchr(flags, 'v') != NULL &&
        ((c >= 0 && c <= 31 && c != '\n' && c != '\t') || c == 127)) {
      printf("^");
      if (c == 127) {
        c = '?';
      } else {
        c = c + 64;
      }
    }
    fputc(c, stdout);
  }
  *prev = c;
}

void print_with_flags(FILE *file, char *flags) {
  int index = 0;
  bool eline_printed = false;
  int prev_char = '\n';
  int current_char;

  while ((current_char = fgetc(file)) != EOF) {
    print_symb(current_char, &prev_char, flags, &index, &eline_printed);
  }
}

int main(int argc, char **argv) {
  char flags[7] = "\0";
  int file_index = -1;

  static struct option long_options[] = {
      {"number-nonblank", no_argument, 0, 'b'},
      {"number", no_argument, 0, 'n'},
      {"squeeze-blank", no_argument, 0, 's'},
      {"show-all", no_argument, 0, 'v'},
      {"show-tabs", no_argument, 0, 't'},
      {"show-ends", no_argument, 0, 'e'},
      {0, 0, 0, 0}};

  int opt;
  while ((opt = getopt_long(argc, argv, "benstv", long_options, NULL)) != -1) {
    switch (opt) {
      case 'b':
      case 'e':
      case 'n':
      case 's':
      case 't':
      case 'v':
        append_flags(flags, opt);
        break;
      default:
        exit(EXIT_FAILURE);
    }
  }

  if (optind < argc) {
    for (int i = optind; i < argc; i++) {
      if (argv[i][0] != '-') {
        file_index = i;
        break;
      }
    }
  }

  if (file_index == -1) {
    printf("No file specified.\n");
    return 1;
  }

  for (int i = file_index; i < argc; i++) {
    if (argv[i][0] != '-') {
      FILE *file;
      if (strcmp("-", argv[i]) == 0) {
        file = stdin;
      } else {
        file = fopen(argv[i], "rt");
      }

      if (file != NULL) {
        print_with_flags(file, flags);
        if (file != stdin) fclose(file);
      } else {
        perror("Error opening or reading the file");
        return 1;
      }
    }
  }

  return 0;
}