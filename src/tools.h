#ifndef TOOLS_H
#define TOOLS_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "options.h"
#include "parse_rule.h"
struct Node_rule;

typedef enum error
{
    NoError,
    NoRule,
    ExecErrorFork, /* exec fails at fork exit 2*/
    ExecErrorExecl, /* exec fails at execl exit 2*/
    ExecError /* exec fails at execution exit 2*/
} error;

void free_string(char **rules);
int rule_exist(char **rules, char *find, int n);
char **find_rules (char *list[], int n);
void dump_rules(char **rules);
int last_modif(char *path);
void handle_return(enum error val);
void free_resources(char **rules, struct Node_rule **nodes);

#endif
