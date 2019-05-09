#ifndef EXECUTE_NODE_H
#define EXECUTE_NODE_H

#include <unistd.h>
#include "sys/wait.h"
#include "parse_rule.h"

void execute_node(struct Node_rule* n);
void exec_all_rules(char **rules, struct Node_rule **nodes);

#endif
