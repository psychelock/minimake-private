#ifndef PARSE_VAR_H
#define PARSE_VAR_H

#include "parse.h"
struct Node_var
{
    char *name;
    char *value;
};

struct Node_var *create_node_var(char *line);
void free_all_node_var(struct Node_var **vars);
#endif
