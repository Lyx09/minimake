#ifndef OPT_PARSER_H
#define OPT_PARSER_H

struct options
{
    char* filename;     // filename of the makefile
    int nonopts;    // index in argv of nonopts
};

struct options opt_parse(int argc, char* argv[]);

#endif /* ! OPT_PARSER_H */
