CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11
CURDIRGREP = ./grep
CURDIRCAT = ./cat

grep: $(CURDIRGREP)/s21_grep

$(CURDIRGREP)/s21_grep: $(CURDIRGREP)/s21_grep.c
	cd $(CURDIRGREP) && $(CC) $(CFLAGS) -o s21_grep s21_grep.c
	cd $(CURDIRGREP) && $(CC) $(CFLAGS) s21_grep.h

cat: $(CURDIRCAT)/s21_cat

$(CURDIRCAT)/s21_cat: $(CURDIRCAT)/s21_cat.c
	cd $(CURDIRCAT) && $(CC) $(CFLAGS) -o s21_cat s21_cat.c

clean:
	rm -f $(CURDIRGREP)/s21_grep
	rm -f $(CURDIRCAT)/s21_cat