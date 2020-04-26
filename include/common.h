#ifndef COMMON_H
#define COMMON_H

#define RC_ERROR    0x2
#define RC_SUCCESS  0x0

#define TRUE    0x1
#define FALSE   0x0

#define COMMENT_CHAR '#'

#include "parser.h"

char *load_var(const struct var *v);

int chr_in_str(const char *s, const char c);

void trim(char *s, const char *trim_chars);

int is_valid_token(char *token, const char *exclude);

char *split_line(char *line, char c);

void rm_comment(char *line);

void rm_trailing_nl(char *line);

void rm_leading(char *line, char *trim_chars);

#endif /* ! COMMON_H */
