#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "exec.h"

#include "parser.h"
#include "vector.h"

// return -1 if target was not found, -2 if an error occured, 1 otherwise
int exec_target(char *target, struct vector *targets, struct vector *vars)
{
    printf("DEBUG: Executing target: %s\n", target);
    // TODO: Handle target duplication ie.
    // mnk will never build the same target twice during the same invocation

    // TODO: Handle variable substitution
    // Find target first
    for (size_t i = 0; i < targets->size; i++)
    {
        struct target *t = vector_get(targets, i);
        struct vector *deps = t->dependencies;
        
        if (strcmp(t->name, target))
            continue;
        
        // Handle dependencies

        // TODO: Fix circular dependencies
        // TODO: Check recency
        // Recursivity yay!
        for (size_t j = 0; j < deps->size; j++)
            exec_target(vector_get(deps, j), targets, vars);

        // Execute commands
        struct vector *cmds = t->commands;
        for (size_t j = 0; j < cmds->size; j++)
        {
            // Logging
            char *command = vector_get(cmds, j);
            if (command[0] != '@')
            {
                printf("%s\n", command);
                fflush(stdout);
            }

            // Execution
            int pid = fork();
            if (pid == 0)
            {
                char *cmd_argv[2];
                cmd_argv[0] = command;
                cmd_argv[1] = NULL;

                execv("/bin/sh -c", cmd_argv);
                // An error occured if this line is reached
            }
            else
            {
                int wstatus;
                waitpid(pid, &wstatus, 0);
                if (WEXITSTATUS(wstatus) != 0)
                    return -2;
            }
        }

        return 1;
    }
    return -1;
}
