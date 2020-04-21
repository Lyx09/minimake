#include <stdlib.h>
#include <stdio.h>
#define _GNU_SOURCE
#include <unistd.h>
#include <getopt.h>

#include "opt_parser.h"

void usage_message(char* binname)
{
    fprintf(stderr, "Usage: %s [options] [target] ...\n", binname); 
    fprintf(stderr, "Options:\n");
    fprintf(stderr, "  -f FILE             Read FILE as a makefile.\n");
    fprintf(stderr, "  -h                  Print this message and exit.\n");

    return;
}

char* opt_parse(int argc, char* argv[])
{
    char* filename;

    // TODO: -f is also equal to longopt --file or --makefile
    // same for --help

    char opt;
    while ((opt = getopt(argc, argv, "hf:")) != -1)
    {
        switch (opt)
        {
            case 'h':
                usage_message(argv[0]);
                exit(EXIT_SUCCESS);
                break;
            case 'f':
                // only the last one counts
                filename = optarg;
                break;
            default:  // '?'
                usage_message(argv[0]);
                exit(2);
        }
    }

    // targets to execute
    while (optind < argc)
    {
        optind++;
    }

    optind = 1; // Reset optind for later parsing
    return filename;
}

