#ifndef EXECUTE_NODE_H
#define EXECUTE_NODE_H

#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>
#include "parse_rule.h"

void execute_node(struct Node_rule* n);
void exec_list(char **rules, struct Node_rule **nodes, int parent);
//void execute_all_dep(char *target, char **depend, struct Node_rule **nodes
//                        ,int parent);

#endif
