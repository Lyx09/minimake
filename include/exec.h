#ifndef EXEC_H
#define EXEC_H

#include "vector.h"
#include "parser.h"

int exec_target(char *target, struct vector *targets, struct vector *vars);

#endif /* ! EXEC_H */
