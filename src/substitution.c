#include <string.h>
#include <stdlib.h>

#include "substitution.h"
#include "parser.h"
#include "vector.h"

extern char **environ;

// Returns the new string with variables substituted to replace the given one
// frees the given one in the process. The returned string will need to be
// freed. In case of parsing error returns NULL
char *var_substitution(char *line, struct vector *vars)
{
    // TODO: Don't forget environment variables !
    (void) line;
    (void) vars;

    //FIXME

    return line;
}
 
char *spec_var_substitution(char *line, struct targets *t)
{

    (void) line;
    (void) t;

    int i = 0;
    // FIXME
    switch (line[i])
    {
        case '@':
            break;

        case '<':
            break;

        case '^':
            break;

        default:
            break;
    }

    return line;
}
