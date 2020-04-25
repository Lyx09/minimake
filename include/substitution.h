#ifndef SUBSTITUTION_H
#define SUBSTITUTION_H

#include "parser.h"
#include "vector.h"

char *var_substitution(char *line, struct vector *vars);

char *spec_var_substitution(char *line, struct targets *t);

#endif /* ! SUBSTITUTION_H*/
