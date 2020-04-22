#ifndef PARSER_H
#define PARSER_H

// I use chained list but I could've used simple arrays
struct target_list
{
    char* name;
    char* dependencies;
    char* commands;
    struct target_list* next;
};

struct var_list
{
    char* name;
    char* value;
    struct var_list* next;
};

int parse(const char* filename, struct target_list* tl, struct var_list* vl);

#endif /* ! PARSER_H */
