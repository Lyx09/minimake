#define _DEFAULT_SOURCE // To use putenv
#define _GNU_SOURCE     // program_invocation_short_name

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "common.h"
#include "exec.h"
#include "opt_parser.h"
#include "parser.h"
#include "pretty_print.h"
#include "vector.h"

extern char *program_invocation_short_name;

void free_vars(struct vector *vars)
{
    for (size_t i = 0; i < vars->size; i++)
    {
        struct var *v = vector_get(vars, i);
        free(v->line);
        free(v);
    }
    vector_destroy(vars);
    return;
}

void free_targets(struct vector *targets)
{
    for (size_t i = 0; i < targets->size; i++)
    {
        struct target *t = vector_get(targets, i);
        struct vector *commands = t->commands;
        for (size_t j = 0; j < commands->size; j++)
            free(vector_get(commands, j));

        vector_destroy(t->dependencies);
        vector_destroy(commands);
        free(t->line);
        free(t);
    }
    vector_destroy(targets);
    return;
}

void free_and_exit(struct vector *targets, struct vector *vars, int rc)
{
    free_vars(vars);
    free_targets(targets);
    exit(rc);

    return;
}

// TODO: Move fprintf in exec
void test_exec_ret_code(int ret, char *target, struct vector *targets,
        struct vector *vars, char *filename)
{
    if (ret < 0)
    {
        switch (ret)
        {
            case -2:
                // This message is not exactly as the make one
                fprintf(stderr, "%s: *** [%s: %s] Error return code was not 0",
                        program_invocation_short_name, filename, target);
                break;
            case -1:
                fprintf(stderr, "%s: *** No rule to make target '%s'. Stop.\n",
                        program_invocation_short_name, target);
                break;
            default:
                fprintf(stderr, "ERROR: This case should not be reached !");
                break;
        }
        free_and_exit(targets, vars, RC_ERROR);
    }
}

// TODO: Handle envp
int main(int argc, char *argv[]) //, char *envp[])
{
    struct options opts = opt_parse(argc, argv);

    // MAKEFILE SELECTION
    char filename[FILENAME_MAX + 1];
    memset(filename, 0, FILENAME_MAX + 1);
    if (opts.filename)
        strncpy(filename, opts.filename, FILENAME_MAX);
    else if(!strlen(filename) && !access("makefile", F_OK))
        strcpy(filename, "makefile");
    else if(!strlen(filename) && !access("Makefile", F_OK))
        strcpy(filename, "Makefile");
    else
    {
        if (opts.nonopts < argc)
        {
            fprintf(stderr,
                    "%s: *** No rule to make target '%s'.  Stop.\n",
                    program_invocation_short_name, argv[opts.nonopts]);
            exit(RC_ERROR);
        }
        else
        {
            fprintf(stderr,
                    "%s: *** No targets specified and no makefile found.  "
                    "Stop.\n", program_invocation_short_name);
            exit(RC_ERROR);
        }
    }


    struct vector *targets = vector_init(10);   // vector of struct target*
    struct vector *vars = vector_init(10);      // vector of struct var*

    // PARSING
    if (parse(filename, targets, vars) == -1)
        free_and_exit(targets, vars, RC_ERROR);

    // PRETTY PRINT
    if (opts.flags & FLAG_PRETTY_PRINT)
    {
        pretty_print(targets, vars);
        free_and_exit(targets, vars, RC_SUCCESS);
    }


    // TARGET SELECTION
    if (opts.nonopts == argc) // No target specified, calling first one
    {
        // TODO: Skip pattern targets "%"
        if (vector_is_empty(targets))
        {
           fprintf(stderr, "%s: *** No targets.  Stop.\n",
                   program_invocation_short_name);
           free_and_exit(targets, vars, RC_ERROR);
        }

        struct target *target = vector_peek_head(targets);
        int ret = exec_target(target->name, targets, vars);
        test_exec_ret_code(ret, target->name, targets, vars, filename);
    }
    else
    {
        for (; opts.nonopts < argc; opts.nonopts++)
        {
            int ret = exec_target(argv[opts.nonopts], targets, vars);
            test_exec_ret_code(ret, argv[opts.nonopts], targets, vars,
                    filename);
        }
    }

    free_and_exit(targets, vars, RC_SUCCESS);
    return RC_SUCCESS; //useless
}
