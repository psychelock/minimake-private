#ifndef PARSE_H
#define PARSE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void parse_makefile(FILE *input);
void parse_error(char *err, char* msg);
#endif

