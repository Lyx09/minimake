#ifndef PARSER_H
#define PARSER_H

#include "vector.h"

struct target
{
    char* line;
    size_t name;         // index in line
    struct vector dependencies; // strings
    struct vector commands;     // char*
};

struct var
{
    char* line;
    size_t name;         // index in line
    size_t value;        // index in line
};

int line_is_empty(char* line);

int parse(const char* filename, struct vector* targets, struct vector* vars);

#endif /* ! PARSER_H */
