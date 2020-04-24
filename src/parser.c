#define _GNU_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "parser.h"
#include "vector.h"
#include "common.h"

// Tells if a line is empty, is var def, target or recipe
// Trailing newline should be removed before calling line_type()
int line_type(char *line)
{
    int is_empty = TRUE;
    for (int i = 0; line[i] && line[i] != COMMENT_CHAR; i++)
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

// 1 if everything parsed ok -1 otherwise
int parse_var_def(struct vector *vars, char *line)
{
    struct var *v = malloc(sizeof(struct var));
    vector_append(vars, v);
    v->line = line;

    char *value = split_line(line, '=');
    if (!value)
        return 0;

    trim(line, " \t");
    // TODO: Make it a function
    // Remove leading whitespace but not trailing ones
    int i = 0;
    int len = strlen(value);
    for (; value[i] && chr_in_str(" \t", value[i]); i++)
        continue;
    memmove(value, value + i, len - i);
    value[len - i] = '\0';

    v->name = line;
    v->value = value;

    if (! is_valid_token(v->name, " \t:#="))
        return 0;

    return 1;
}

// nb of cmds if everything parsed ok -1 otherwise
int parse_target_def(struct vector *targets, char *line, FILE *makefile)
{
    struct target *t = malloc(sizeof(struct target));
    vector_append(targets, t);
    struct vector *deps = vector_init(10);
    struct vector *cmds = vector_init(10);
    t->dependencies = deps;
    t->commands = cmds;
    t->line = line;

    char *deps_str = split_line(line, ':');
    if (!deps_str)
        return 0;

    trim(line, " \t");
    trim(deps_str, " \t");

    t->name = line;
    if (!is_valid_token(t->name, " \t:#="))
        return 0;


    // DEPENDENCIES
    char *dep;
    for (;; deps_str = NULL)
    {
        dep = strtok(deps_str, " \t");
        if (dep == NULL)
            break;
        vector_append(deps, dep);
    }

    // COMMANDS
    char *l = NULL;
    size_t len = 0;
    int line_nb = 0;
    for (ssize_t nb_bytes = 0;
            (nb_bytes = getline(&l, &len, makefile)) != -1;
            line_nb++)
    {
        nb_bytes = rm_trailing_nl(l, nb_bytes);
        if (line_type(l) == LINE_EMPTY)
        {
            free(l);
            l = NULL;
            len = 0;
            continue;
        }


        if (l[0] != '\t')
        {
            fseek(makefile, -nb_bytes, SEEK_CUR); // Forget this line
            line_nb--;
            break;
        }

        // Remove leading whitespace but not trailing ones
        int i = 0;
        for (; l[i] && chr_in_str(" \t", l[i]); i++)
            continue;
        memmove(l, l + i, nb_bytes - i);
        l[nb_bytes - i] = '\0';

        vector_append(cmds, l);
        l = NULL;
        len = 0;
    }
    free(l); // Don't forget this one !

    return line_nb;
}

// Returns 1 if everythin went right -1 otherwise
int parse(const char *filename, struct vector *targets, struct vector *vars)
{
    (void) targets;
    FILE *makefile = fopen(filename, "r");

    char *line = NULL;
    size_t len = 0;
    int line_nb = 1;

    int error = FALSE;

    for (ssize_t nb_bytes = 0; 
            !error && (nb_bytes = getline(&line, &len, makefile)) != -1;
            line_nb++)
    { 

        // multiline should be handled here
        rm_comment(line);
        rm_trailing_nl(line, nb_bytes);

        switch (line_type(line))
        {
            case LINE_EMPTY:
                free(line);
                break;
            case LINE_VAR_DEF:
                if (parse_var_def(vars, line) == -1)
                    error = TRUE;
                break;
            case LINE_TARGET_DEF:
                {
                    int ret = parse_target_def(targets, line, makefile);
                    if (ret == -1 )
                        error = TRUE;
                    else
                        line_nb += ret;
                    break;
                }
            case LINE_OTHER:
                fprintf(stderr, "%s:%d: *** missing separator.  Stop.\n",
                        filename, line_nb);
                free(line);
                error = TRUE;
                break;
            default:
                fprintf(stderr, "Error: Case should not be reached !\n");
                free(line);
                error = TRUE;
                break;
        }
        line = NULL;
        len = 0;
    }

    free(line); // Don't forget this one !
    fclose(makefile);
    return 1;
}
