#define _DEFAULT_SOURCE // To use putenv

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"

// Wrapper on putenv. Load a variable v into the env
char *load_var(const struct var *v)
{
    char* var_text = malloc(strlen(v->name) + strlen(v->value) + 4);
    strcpy(var_text, v->name);
    strcat(var_text, "=");
    strcat(var_text, v->value);

    putenv(var_text);

    return var_text;
}


// Simple wrapper
int chr_in_str(const char *s, const char c)
{
    return strchr(s, c) && 1;
}

// Modify s so that only the interesting part is kept in front
// trim_char will usually be "\t "
void trim(char *s, const char *trim_chars)
{
    int end = strlen(s);
    int begin = 0;
    for (; begin < end && chr_in_str(trim_chars, s[begin]); begin++)
        continue;
    for (; end >= begin && chr_in_str(trim_chars, s[end]); end--)
        continue;
    memmove(s, s + begin, end - begin + 1);
    s[end - begin + 1] = '\0';
}

// checks if any character in exclude is in token
int is_valid_token(char *token, const char *exclude)
{
    if (! token[0])
        return FALSE;
    for (; *token; token++)
        if (chr_in_str(exclude, *token))
                return FALSE;
    return TRUE;
}

// splits a line on a given char. Returns the second part of the split or
// NULL if none or several split characters have been found
char *split_line(char *line, char c)
{
    char *split = strchr(line, c);
    if (!split)
        return NULL;
    if (split != strrchr(line, c))
        return NULL;
    *split = '\0';
    return split +1;
}

// FIXME handle escaped COMMENT_CHAR or in ""
void rm_comment(char *line)
{
    char *com = strchr(line, COMMENT_CHAR);
    if (com != NULL)
        *com = '\0';
    return;
}

// TODO: Replace by rm_trailing instead ?
void rm_trailing_nl(char *line)
{
    int len = strlen(line);
    if (line[len - 1] == '\n')
        line[len - 1] = '\0';
    return;
}

void rm_leading(char *line, char *trim_chars)
{
    int i = 0;
    int len = strlen(line);
    for (; line[i] && chr_in_str(trim_chars, line[i]); i++)
        continue;
    memmove(line, line + i, len - i);
    line[len - i] = '\0';

    return;
}
