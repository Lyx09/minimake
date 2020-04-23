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
