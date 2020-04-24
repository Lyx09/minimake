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


int target_exists(char *target, struct vector *targets)
{
    for (size_t i = 0; i < targets->size; i++)
        if (! strcmp(((struct target*)vector_get(targets, i))->name, target))
            return TRUE;
    return FALSE;
}

int main(int argc, char *argv[])
{
    struct options opts = opt_parse(argc, argv);

    // MAKEFILE SELECTION
    char filename[FILENAME_MAX + 1];
    memset(filename, 0, FILENAME_MAX + 1);
    if (opts.filename)
        strncpy(filename, opts.filename, FILENAME_MAX);
    if(!strlen(filename) && !access("makefile", F_OK))
        strcpy(filename, "makefile");
    if(!strlen(filename) && !access("Makefile", F_OK))
        strcpy(filename, "Makefile");
    if(!strlen(filename))
    {
        if (opts.nonopts < argc)
        {
            fprintf(stderr,
                    "%s: *** No rule to make target '%s'.  Stop.\n",
                    argv[0], argv[opts.nonopts]);
            exit(RC_ERROR);
        }
        else
        {
            fprintf(stderr,
                    "%s: *** No targets specified and no makefile found.  "
                    "Stop.\n",
                    argv[0]);
            exit(RC_ERROR);
        }
    }


    struct vector *targets = vector_init(10);   // vector of struct target*
    struct vector *vars = vector_init(10);      // vector of struct var*

    // PARSING
    if (parse(filename, targets, vars) == -1)
    {
        free_vars(vars);
        free_targets(targets);
        exit(RC_ERROR);
    }

    // PRETTY PRINT
    if (opts.flags & FLAG_PRETTY_PRINT)
    {
        pretty_print(targets, vars);
        free_vars(vars);
        free_targets(targets);
        exit(RC_SUCCESS);
    }


    // TARGET SELECTION
    if (opts.nonopts == argc) // No target specified, calling first one
    {
        // TODO: Skip pattern targets "%"
        if (vector_is_empty(targets))
        {
           fprintf(stderr, "%s: *** No targets.  Stop.\n", argv[0]);
           free_vars(vars);
           free_targets(targets);
           exit(RC_ERROR);
        }

        exec_target(((struct target*)vector_peek_head(targets))->name,
                targets, vars);
    }
    else
    {
        for (; opts.nonopts < argc; opts.nonopts++)
        {
            if (! target_exists(argv[opts.nonopts], targets))
            {
                fprintf(stderr, "%s: *** No rule to make target '%s'. \n",
                        argv[0], argv[opts.nonopts]);
                free_vars(vars);
                free_targets(targets);
                exit(RC_ERROR);
            }
            else
                exec_target(argv[opts.nonopts], targets, vars);
        }
    }

    // some free time
    free_vars(vars);
    free_targets(targets);

    return RC_SUCCESS;
}
