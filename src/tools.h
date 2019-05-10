#ifndef TOOLS_H
#define TOOLS_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "options.h"

void free_string(char **rules);
char **find_rules (char *list[], int n);
void dump_rules(char **rules);
struct tm* last_modif(char *path);

#endif
