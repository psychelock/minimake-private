#ifndef PARSE_RULE_H
#define PARSE_RULE_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "parse.h"
#include "options.h"
struct Node_rule
{
    char *target;
    char **depend;
    //char **depend_file;
    //node_rule **depend_rule;
    char **recipe;
};

struct Node_rule *create_node_rule(char *line1, FILE *input);
void dump_node_rule(struct Node_rule* n);
void free_node_rule(struct Node_rule *n);

#endif
