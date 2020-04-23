#include <stdio.h>

#include "pretty_print.h"
#include "vector.h"
#include "parser.h"

void pretty_print(struct vector* targets, struct vector* vars)
{
    printf("# variables\n");

    // FIXME: !!! Add variable expansion
    for (int i = 0; i < vars->size; i++)
    {
        struct var *v = vector_get(vars, i);
        printf("'%s' = '%s'\n", v->name, v->value);
    }

    printf("# rules\n");
    for (int i = 0; i < targets->size; i++)
    {
        struct target *t = vector_get(targets, i);
        struct vector *deps = t->dependencies;
        struct vector *cmds = t->commdands;

        printf("(%s):", t->name);
        for (int j = 0; j < deps->size; j++)
            printf(" [%s]", vector_get(deps, j));

        printf("\n");

        for (int j = 0; j < cmds->size; j++)
            printf("\t'%s'\n", vector_get(cmds, j));
    }
    return;
}

