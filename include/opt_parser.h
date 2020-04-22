#ifndef OPT_PARSER_H
#define OPT_PARSER_H

#define FLAG_PRETTY_PRINT   0x1

struct options
{
    char* filename;     // filename of the makefile
    int nonopts;    // index in argv of nonopts
    int flags;
};

struct options opt_parse(int argc, char* argv[]);

#endif /* ! OPT_PARSER_H */
