#include <stdlib.h>
#include <stdio.h>
#define _GNU_SOURCE
#include <unistd.h>
#include <getopt.h>

#include "opt_parser.h"
#include "common.h"

void usage_message(char* binname)
{
    fprintf(stderr, "Usage: %s [options] [target] ...\n", binname); 
    fprintf(stderr, "Options:\n");
    fprintf(stderr, "  -f FILE             Read FILE as a makefile.\n");
    fprintf(stderr, "  -h                  Print this message and exit.\n");
    fprintf(stderr, "  -p                  Pretty print.\n");

    return;
}

struct options opt_parse(int argc, char* argv[])
{
    char* filename = NULL;
    int flags = 0;

    // TODO: -f is also equal to longopt --file or --makefile
    // same for --help

    char opt;
    while ((opt = getopt(argc, argv, "phf:")) != -1)
    {
        switch (opt)
        {
            case 'p':
                flags |= FLAG_PRETTY_PRINT;
                break;
            case 'h':
                usage_message(argv[0]);
                exit(RC_SUCCESS);
                break;
            case 'f':
                // only the last one counts
                filename = optarg;
                break;
            default:  // '?'
                usage_message(argv[0]);
                exit(RC_ERROR);
        }
    }

    int nonopts = optind;
    optind = 1; // Reset optind for later parsing
    return (struct options){filename, nonopts, flags};
}

