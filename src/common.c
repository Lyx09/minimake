#include <stdio.h>
#include <string.h>

#include "common.h"

// Simple wrapper
int chr_in_str(const char *s, const char c)
{
    return strchr(s, c) && 1;
}

// Modify s so that only the interesting part is kept in front
// trim_char will usually be "\t "
void trim(char *s, const char* trim_char)
{
    int end = strlen(s);
    int begin = 0;
    for (; chr_in_str(trim_char, s[begin]); begin++)
        continue;
    for (; chr_in_str(trim_char, s[end]); end--)
        continue;
    memmove(s, s + begin, end - begin + 1);
    s[end - begin + 1] = '\0';
}

// checks if any character in exclude is in token
int is_valid_token(char *token, const char *exclude)
{
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

void rm_trailing_nl(char *line, int len)
{
    if (line[len - 1] == '\n')
        line[len - 1] = '\0';
    return;
}
