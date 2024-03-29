#define _GNU_SOURCE     // program_invocation_short_name

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#include "exec.h"

#include "parser.h"
#include "common.h"
#include "vector.h"
#include "substitution.h"

extern char *program_invocation_short_name;
extern char **environ;

int deps_older_than_target(struct target *t)
{
    struct vector *deps = t->dependencies;
    for (size_t i = 0; i < deps->size; i++)
    {
        //FIXME
    }
    return TRUE;
}


// return -1 if target was not found, -2 if an error occured, 1 otherwise
int exec_target(char *target, struct vector *targets)
{
    // TODO: Handle target duplication ie.
    // mnk will never build the same target twice during the same invocation

    // TODO: Handle variable substitution
    // Find target first
    struct target *t;
    for (size_t i = 0; i < targets->size; i++)
    {
        t = vector_get(targets, i);
        if (!strcmp(t->name, target))
            break;
    }

    // target not found
    if (strcmp(t->name, target))
    {
        fprintf(stderr, "%s: *** No rule to make target '%s'. Stop.\n",
                program_invocation_short_name, target);
        return -1;

    }

    // Target already up to date
    if (t->ran_once || (! access(t->name, F_OK) && deps_older_than_target(t)))
    {
        printf("%s: '%s' is up to date.\n", program_invocation_short_name,
                target);
        return 1;
    }

    // Handle dependencies

    // TODO: Fix circular dependencies


    // Recursivity yay!
    struct vector *deps = t->dependencies;
    for (size_t j = 0; j < deps->size; j++)
    {
        if (exec_target(vector_get(deps, j), targets) < 0)
            return -1;
    }

    // Execute commands
    struct vector *cmds = t->commands;
    for (size_t j = 0; j < cmds->size; j++)
    {
        // Logging
        char *command = vector_get(cmds, j);
        command = var_substitution(command);
        vector_replace(cmds, j, command);
        if (command[0] != '@')
        {
            printf("%s\n", command);
            fflush(stdout);
        }

        // Execution
        int pid = fork();
        if (pid == 0)
        {
            char *cmd_argv[] = {"/bin/sh", "-c", command, NULL};
            execve(cmd_argv[0], cmd_argv, environ);
            // An error occured if this line is reached
        }
        else
        {
            int wstatus;
            waitpid(pid, &wstatus, 0);
            if (WEXITSTATUS(wstatus) != 0)
            {
                // This message is not exactly as the make one
                fprintf(stderr, "%s: *** Error return code was not 0\n",
                        program_invocation_short_name);
                return -2;
            }
        }
    }

    t->ran_once = TRUE;
    return 1;

}
