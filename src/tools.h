#ifndef TOOLS_H
#define TOOLS_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "parse.h"
#include "options.h"
#include "parse_rule.h"
#include "parse_var.h"

struct Node_rule;
struct AllNodes;

typedef enum error
{
    Invalid,
    NoError,
    NoCommand,
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
enum error handler(enum error val1, enum error val2);
int file_exist(char *path);
void free_resources(char **rules, struct AllNodes *allnodes);
int handle_var_error(char *line);
void empty_string(char *string);

#endif
