#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100000

struct options {
  int e;
  int i;
  int c;
  int v;
  int l;
  int n;
  int h;
  int s;
  int o;
  int f;
  int c_flag;
  char str[MAX];
  char str_o[MAX];
  char namefile[MAX];
  char array[MAX];
};

void flag_parse(int argc, char **argv, struct options *opt);
void output(char **argv, struct options *opt);
void f_flag(struct options *opt);
