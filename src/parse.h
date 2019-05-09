#ifndef PARSE_H
#define PARSE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parse_rule.h"

struct Node_rule** parse_makefile(FILE *input);
void parse_error(char *err, char* msg);
#endif

