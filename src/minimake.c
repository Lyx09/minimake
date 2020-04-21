#include <stdio.h>

#include "lexer.h"
#include "parser.h"
#include "exec.h"
#include "opt_parser.h"


int main(int argc, char* argv[])
{
    opt_parse(argc, argv);

    return 0;
}
