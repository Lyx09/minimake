#include "parser.h"
#include "vector.h"

// Returns the new string with variables substituted to replace the given one
// frees the given one in the process. The returned string will need to be
// freed. In case of parsing error returns NULL
char *var_substitution(char *line, struct vector *vars)
{
    (void) line;
    (void) vars;

    //FIXME

    return line;
}
