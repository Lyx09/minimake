#include <stdio.h>

#include "pretty_print.h"
#include "vector.h"
#include "parser.h"

void pretty_print(struct vector *targets, struct vector *vars)
{
    printf("# variables\n");

    // FIXME: !!! Add variable expansion
    for (size_t i = 0; i < vars->size; i++)
    {
        struct var *v = vector_get(vars, i);
        printf("'%s' = '%s'\n", v->name, v->value);
    }

    printf("# rules\n");
    for (size_t i = 0; i < targets->size; i++)
    {
        struct target *t = vector_get(targets, i);
        struct vector *deps = t->dependencies;
        struct vector *cmds = t->commands;

        printf("(%s):", t->name);
        for (size_t j = 0; j < deps->size; j++)
        {
            char *dep = vector_get(deps, j);
            printf(" [%s]", dep);
        }

        printf("\n");

        for (size_t j = 0; j < cmds->size; j++)
        {
            char *cmd = vector_get(cmds, j);
            printf("\t'%s'\n", cmd);
        }
    }
    return;
}

