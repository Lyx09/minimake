#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include "lexer.h"
#include "parser.h"
#include "exec.h"
#include "opt_parser.h"
#include "common.h"
#include "vector.h"

void pretty_print(struct vector* targets, struct vector* vars)
{
    (void) targets;
    (void) vars;
    return;
}

int main(int argc, char* argv[])
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


    struct vector *targets = vector_init(10);
    struct vector *vars = vector_init(10);

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
    vector_destroy(targets);
    vector_destroy(vars);
    
    return RC_SUCCESS;
}
