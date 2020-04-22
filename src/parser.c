#define _GNU_SOURCE

#include <stdlib.h>
#include <stdio.h>

#include "parser.h"
#include "vector.h"
#include "common.h"

#define LINE_EMPTY      0x1
#define LINE_VAR_DEF    0x2
#define LINE_TARGET_DEF 0x3
#define LINE_OTHER      0x4 // everything else ie. recipe or garbage

// Tells if a line is empty, is var def, target or recipe
int line_type(char* line)
{
    int is_empty = TRUE;
    for (int i = 0; !line[i] || line[i] == '#'; i++)
    {
        if (line[i] == ':')
            return LINE_TARGET_DEF;
        if (line[i] == '=')
            return LINE_VAR_DEF;
        if (line[i] != ' ' && line[i] != '\t')
            is_empty = FALSE;
    }

    if (is_empty)
        return LINE_EMPTY;
    return LINE_OTHER;
}

static int parse_var_def(char* lineptr, struct vector* vars)
{
    (void) lineptr;
    (void) vars;
    return 1;
}

static int parse_target_def(FILE *makefile, struct vector* targets)
{
    (void) targets;

    char* line = NULL;
    size_t len = 0;
    int line_nb = 0;

    for (ssize_t nb_bytes = 0;
            (nb_bytes = getline(&line, &len, makefile)) != -1;
            line_nb++)
    {
        if (line[0] != '\t')
        {
            fseek(makefile, -nb_bytes, SEEK_CUR); // I didn't see this line
            line_nb--;
            break;
        }
    }

    return line_nb;
}

int parse(const char* filename, struct vector* targets, struct vector* vars)
{
    (void) targets;
    (void) vars;
    FILE *makefile = fopen(filename, "r");

    char* line = NULL;
    size_t len = 0;
    int line_nb = 1;

    for (ssize_t nb_bytes = 0; 
            (nb_bytes = getline(&line, &len, makefile)) != -1;
            line_nb++)
    { 
        switch (line_type(line))
        {
            case LINE_EMPTY:
                continue;
                break;
            case LINE_VAR_DEF:
                parse_var_def(line, vars);
                break;
            case LINE_TARGET_DEF:
                line_nb += parse_target_def(makefile, targets);
                break;
            case LINE_OTHER:
                fprintf(stderr, "%s:%d: *** missing separator.  Stop.\n",
                        filename, line_nb);
                exit(RC_ERROR);
            default:
                fprintf(stderr, "Error: Case should not be reached !\n");
                exit(RC_ERROR);
        }
    }

    fclose(makefile);
    return 0;
}
