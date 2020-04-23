#define _GNU_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "parser.h"
#include "vector.h"
#include "common.h"

// Tells if a line is empty, is var def, target or recipe
int line_type(char *line)
{
    int is_empty = TRUE;
    for (int i = 0; !line[i]; i++)
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
    trim(value, " \t\n");

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
    trim(deps_str, " \t\n");

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
        if (l[0] != '\t')
        {
            fseek(makefile, -nb_bytes, SEEK_CUR); // Forget this line
            line_nb--;
            break;
        }

        trim(l, " \t\n");
        vector_append(cmds, l);
        l = NULL;
        len = 0;
    }

    return line_nb;
}

int parse(const char *filename, struct vector *targets, struct vector *vars)
{
    (void) targets;
    FILE *makefile = fopen(filename, "r");

    char *line = NULL;
    size_t len = 0;
    int line_nb = 1;
    for (ssize_t nb_bytes = 0; 
            (nb_bytes = getline(&line, &len, makefile)) != -1;
            line_nb++)
    { 
        // multiline should be handled here
        remove_comment(line);

        switch (line_type(line))
        {
            case LINE_EMPTY:
                continue;
                break;
            case LINE_VAR_DEF:
                // FIXME: Check RC
                parse_var_def(vars, line);
                line = NULL;
                len = 0;
                break;
            case LINE_TARGET_DEF:
                // FIXME: Check RC
                line_nb += parse_target_def(targets, line, makefile);
                line = NULL;
                len = 0;
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
