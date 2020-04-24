#include <stdio.h>
#include <string.h>

#include "exec.h"

#include "parser.h"
#include "vector.h"

// return -1 if target was not found, 1 otherwise
int exec_target(char *target, struct vector *targets, struct vector *vars)
{
    printf("DEBUG: Executing target: %s\n", target);

    // TODO: Handle variable substitution
    // Find target first
    for (size_t i = 0; i < targets->size; i++)
    {
        if (strcmp(((struct target*)vector_get(targets, i))->name, target))
            continue;
        
        // Handle dependencies
        struct target *t = vector_get(targets, i);
        struct vector *deps = t->dependencies;

        // TODO: Fix circular dependencies
        // TODO: Check recency
        // Recursivity yay!
        for (size_t j = 0; j < deps->size; j++)
            exec_target(vector_get(deps, j), targets, vars);

        // Execute commands
        struct vector *cmds = t->commands;
        for (size_t j = 0; j < cmds->size; j++)
        {
            char *command = vector_get(cmds, j);
            if (command[0] != '@')
            {
                printf("%s", command);
                fflush(stdout);
            }

        }


        return 1;
    }

    return -1;
}
