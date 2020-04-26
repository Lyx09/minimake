#ifndef SUBSTITUTION_H
#define SUBSTITUTION_H

#include "parser.h"
#include "vector.h"

struct subst
{
    int index; // Location of the $ character
    int length; // Total length of the variable to substitute '{}()$' included
    const char *name; // Position of the var name in the string
    int name_len; // Length of the var name
};

char *var_substitution(char *line);

char *spec_var_substitution(char *line, struct target *t);

#endif /* ! SUBSTITUTION_H*/
