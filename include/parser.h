#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>

#include "vector.h"


#define LINE_EMPTY      0x1
#define LINE_VAR_DEF    0x2
#define LINE_TARGET_DEF 0x3
#define LINE_OTHER      0x4     // everything else ie. recipe or garbage


struct target
{
    char *line;                 // malloc'd by getline
    char *name;                 // points to line
    struct vector *dependencies; // vec of char *pointing to line
    struct vector *commands;     // vec of char *malloc'd by getline
    int ran_once;               // For exec: has the target already been run ?
};

struct var
{
    char *line;                 // malloc'd by getline
    char *env;                  // string given to putenv(3)
    char *name;                 // points to line
    char *value;                // points to line
};

int line_type(char *line);

int parse_var_def(struct vector *vars, char *line);

int parse_target_def(struct vector *targets, char *line, FILE *makefile);

int parse(const char *filename, struct vector *targets, struct vector *vars);

#endif /* ! PARSER_H */
