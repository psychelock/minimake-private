#ifndef PARSE_VAR_H
#define PARSE_VAR_H

#include "parse.h"
struct Node_var
{
    char *name;
    char *value;
};

struct Node_var *create_node_var(char *line, int env);
void free_all_node_var(struct Node_var **vars);
struct Node_var *find_node_var(char *name, struct Node_var **vars);
char *remove_front_ws(char *string);
struct Node_var **parse_env_var(char **penv);
#endif
