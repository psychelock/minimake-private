#ifndef PARSE_H
#define PARSE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parse_rule.h"

struct AllNodes
{
    struct Node_rule **nodes;
    struct Node_var **vars;
};

struct AllNodes *parse_makefile(FILE *input);
void parse_error(char *err, char* msg);
#endif

