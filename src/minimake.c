#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include "lexer.h"
#include "parser.h"
#include "exec.h"
#include "opt_parser.h"



int main(int argc, char* argv[])
{
    struct options opts = opt_parse(argc, argv);
    
    char filename[256];
    memset(filename, 0, 256);
    if (opts.filename)
        strncpy(filename, opts.filename, 255);
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

    printf("%s", filename);



    // For later
    //for (; opts.nonopts < argc; opts.nonopts++)
        //exec_target(argv[opts.nonopts]);

    return 0;
}
