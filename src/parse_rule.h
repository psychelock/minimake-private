#ifndef PARSE_RULE_H
#define PARSE_RULE_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "parse.h"
#include "options.h"
#include "tools.h"
#include "parse_var.h"

struct Node_var;
struct Node_rule
{
    char *target;
    char **depend;
    char **recipe;
};

struct Node_rule *create_node_rule(char *line1, FILE *input, struct Node_var **vars);
void dump_node_rule(struct Node_rule* n);
void free_node_rule(struct Node_rule *n);
void free_all_nodes(struct Node_rule **n);

char *remove_back_ws(char *string);


void extract_var(char *res, char delim, char *string, int start);
void find_and_replace(char *dest, char *string, struct Node_var **vars);

#endif
