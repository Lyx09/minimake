#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "common.h"
#include "exec.h"
#include "lexer.h"
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

int main(int argc, char *argv[])
{
    struct options opts = opt_parse(argc, argv);
    
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
        fprintf(stderr,
                "%s: *** No targets specified and no makefile found.  Stop.\n",
                argv[0]);
        exit(2);
    }


    struct vector *targets = vector_init(10);   // vector of struct target*
    struct vector *vars = vector_init(10);      // vector of struct var*

    parse(filename, targets, vars);
    
    if (opts.flags & FLAG_PRETTY_PRINT)
    {
        pretty_print(targets, vars);
        exit(RC_SUCCESS);
    }


    // For later
    //for (; opts.nonopts < argc; opts.nonopts++)
        //exec_target(argv[opts.nonopts]);

    // some free time
    free_vars(vars);
    free_targets(targets);
    
    return RC_SUCCESS;
}
