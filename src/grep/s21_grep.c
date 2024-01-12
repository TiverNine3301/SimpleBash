#include "s21_grep.h"

int main(int argc, char **argv) {
  struct options opt = {0};
  flag_parse(argc, argv, &opt);
  while (optind < argc) {
    output(argv, &opt);
    optind++;
  }
  return 0;
}

void flag_parse(int argc, char **argv, struct options *opt) {
  int g;
  while ((g = getopt_long(argc, argv, "e:ivclnhsf:o", NULL, 0)) != -1) {
    switch (g) {
      case 'e':
        opt->e = 1;
        strcat(opt->array, optarg);
        strcat(opt->array, "|");
        break;
      case 'i':
        opt->i = 1;
        break;
      case 'v':
        opt->v = 1;
        break;
      case 'c':
        opt->c = 1;
        break;
      case 'l':
        opt->l = 1;
        break;
      case 'n':
        opt->n = 1;
        break;
      case 'h':
        opt->h = 1;
        break;
      case 's':
        opt->s = 1;
        break;
      case 'o':
        opt->o = 1;
        break;
      case 'f':
        opt->f = 1;
        strcpy(opt->namefile, optarg);
        f_flag(opt);
        break;
      default:
        exit(1);
        break;
    }
    if (opt->v && opt->o) {
      opt->o = 0;
    }
  }
  if (!opt->e && !opt->f) {
    if (argc > optind) {
      strcat(opt->array, argv[optind]);
    }
    optind++;
  }
  if (opt->e || opt->f) {
    opt->array[strlen(opt->array) - 1] = '\0';
  }
  if (argc - optind > 1) opt->c_flag = 1;
}

void output(char **argv, struct options *opt) {
  FILE *fp;
  regex_t regex;
  regmatch_t regmatch;
  int c_count = 0;
  int str_count = 0;
  int flag_i = REG_EXTENDED;
  if (opt->i) {
    flag_i = REG_EXTENDED | REG_ICASE;
  }
  regcomp(&regex, opt->array, flag_i);
  fp = fopen(argv[optind], "rb");
  if (fp != NULL) {
    while (fgets(opt->str, MAX, fp) != NULL) {
      int str = regexec(&regex, opt->str, 1, &regmatch, 0);
      str_count++;
      if (!str) c_count++;
      if (opt->v) str = !str;
      if (opt->o) strcpy(opt->str_o, opt->str);
      if (!str && opt->c_flag && !opt->l && !opt->h && !opt->c)
        printf("%s:", argv[optind]);
      if (!str && !opt->l && !opt->c && !opt->n && !opt->o) {
        printf("%s", opt->str);
        if (opt->str[strlen(opt->str) - 1] != '\n') printf("\n");
      }
      if (!str && opt->n && !opt->c && !opt->l) {
        if (opt->o) {
          printf("%d:", str_count);
        } else
          printf("%d:%s", str_count, opt->str);
        if (opt->str[strlen(opt->str) - 1] != '\n') printf("\n");
      }
      if (!str && opt->o && !opt->l && !opt->c) {
        char *point = opt->str_o;
        while ((regexec(&regex, point, 1, &regmatch, 0) == 0)) {
          printf("%.*s\n", (int)(regmatch.rm_eo - regmatch.rm_so),
                 point + regmatch.rm_so);
          point += regmatch.rm_eo;
        }
      }
    }
    regfree(&regex);
    if (opt->l && str_count - c_count > 0 && opt->v) {
      printf("%s\n", argv[optind]);
    }
    if (opt->l && c_count > 0 && !opt->c && !opt->v)
      printf("%s\n", argv[optind]);
    if (opt->c && opt->c_flag && !opt->h) printf("%s:", argv[optind]);
    if (opt->c && !opt->l && !opt->v) printf("%d\n", c_count);
    if (opt->c && !opt->l && opt->v) printf("%d\n", str_count - c_count);
    if (opt->c && opt->l) {
      if (c_count > 0) {
        c_count = 1;
        printf("%d\n%s\n", c_count, argv[optind]);
      } else
        printf("%d\n", c_count);
    }
    fclose(fp);
  } else {
    regfree(&regex);
    if (!opt->s)
      fprintf(stderr, "grep: %s: No such file or directory\n", argv[optind]);
  }
}

void f_flag(struct options *opt) {
  FILE *f;
  f = fopen(opt->namefile, "rb");
  if (f != NULL) {
    while (!feof(f)) {
      if (fgets(opt->str, 1000, f) != NULL) {
        if (opt->str[strlen(opt->str) - 1] == '\n' && strlen(opt->str) - 1 != 0)
          opt->str[strlen(opt->str) - 1] = '\0';
        strcat(opt->array, opt->str);
        strcat(opt->array, "|");
      }
    }
    fclose(f);
  }
}
