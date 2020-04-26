#define _GNU_SOURCE

#include <errno.h>
#include <string.h>
#include <stdlib.h>

#include "substitution.h"
#include "parser.h"
#include "vector.h"
#include "common.h"

extern char **environ;
extern char *program_invocation_short_name;

// Finds the first variable in line after index *i
// returns -1 if no var was found, -2 if an error occured, 1 otherwise
int locate_var(const char *line, struct subst *subst, int *i)
{
    for (; line[*i]; (*i)++)
    {
        if (line[*i] != '$')
            continue;

        //'\n'should've already been removed anyway
        if (!line[*i + 1] || line[*i + 1] == '\n') 
            return -1;

        // All error cases are magically aleardy treated
        // As if make did not care about special characters
        subst->index = *i;
        (*i)++;
        if (chr_in_str("{(", line[*i]))
        {
            char matching = (line[*i] == '{' ? '}' : ')');
            subst->name_len = 0;

            (*i)++;
            subst->name = line + *i;
            for (; line[*i] != matching; (*i)++, subst->name_len++)
            {
                if (!line[*i] || line[*i] == '\n')
                {
                    fprintf(stderr, "%s: *** unterminated variable reference."
                            "  Stop.\n", program_invocation_short_name);
                    return -2;
                }
            }
            subst->length = subst->name_len + 3;
            return 1;
        }
        else
        {
            subst->name = line + *i;
            subst->length = 2;
            subst->name_len = 1;
            return 1;
        }
    }

    // No var was found
    return -1;
}


// start: Index of first char after changed var
char *expand_var(char *line, struct subst subst, const char *value, int *start)
{
    if (!value)
        value = "";
    // rp = right part
    int rp_idx = subst.index + subst.length;
    int rp_len = strlen(line + rp_idx);
    int new_rp_idx = subst.index + strlen(value);
    line = realloc(line, new_rp_idx + rp_len + 1);
    // Don't forget the null byte
    memmove(line + new_rp_idx, line + rp_idx, rp_len);
    memmove(line + subst.index, value, strlen(value));
    line[new_rp_idx + rp_len] = '\0';
    *start = new_rp_idx;

    return line;
}

char *getenvn(const char *name, int len)
{
    char *n = calloc(len + 1, sizeof(char));
    strncpy(n, name, len);
    char *ret = getenv(n);
    free(n);
    return ret;
}

// Returns the new string with variables substituted to replace the given one
// frees the given one in the process. The returned string will need to be
// freed. In case of parsing error returns NULL
char *var_substitution(char *line)
{
    struct subst subst;
    int ret = 0;
    int start = 0;
    while ((ret = locate_var(line, &subst, &start)) > 0)
    {
        char *value;
        if (subst.name[0] == '$' && subst.name_len == 1)
            value = "$";
        else
            value = getenvn(subst.name, subst.name_len);
        line = expand_var(line, subst, value, &start);
    }

    if (ret == -2)
        return NULL;
    else
        return line;
}

// Don't forget to free my return value
char *concat_deps(struct vector *deps)
{
    int total_size = 0;
    for (size_t i = 0; i < deps->size; i++)
        total_size += strlen(vector_get(deps, i));

    char *cat = calloc(total_size + deps->size, sizeof(char));

    for (size_t i = 0; i < deps->size; i++)
    {
        strcat(cat, vector_get(deps, i));
        if (i != deps->size - 1)
            strcat(cat, " ");
    }

    return cat;
}

// Used for variable substitution in commands. Returns the new line substitued
// Frees the given line. User must free the returned string;
char *spec_var_substitution(char *line, struct target *t)
{
    struct subst subst;
    int start = 0;
    while (locate_var(line, &subst, &start) > 0)
    {
        if (subst.name_len != 1)
            continue;
        switch (subst.name[0])
        {
            case '@':
                line = expand_var(line, subst, t->name, &start);
                break;

            case '<':
                {
                    char *dep = vector_peek_head(t->dependencies);
                    line = expand_var(line, subst, dep, &start);
                    break;
                }

            case '^':
                {
                    char *cat = concat_deps(t->dependencies);
                    line = expand_var(line, subst, cat, &start);
                    free(cat);
                    break;
                }

            default:
                break;
        }
    }

    return line;
}
