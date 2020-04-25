#ifndef SUBSTITUTION_H
#define SUBSTITUTION_H

#include "parser.h"
#include "vector.h"

char *var_substitution(char *line, struct vector *vars);

#endif /* ! SUBSTITUTION_H*/
