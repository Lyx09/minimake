#include <stdio.h>

#include "lexer.h"
#include "parser.h"
#include "exec.h"
#include "opt_parser.h"


int main(int argc, char* argv[])
{
    struct options opts = opt_parse(argc, argv);
    printf("%s\n", opts.filename);
    for (; opts.nonopts < argc; opts.nonopts++)
    {
        printf("%s\n", argv[opts.nonopts]);
    }


    return 0;
}
