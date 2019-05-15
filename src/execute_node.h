#ifndef EXECUTE_NODE_H
#define EXECUTE_NODE_H

#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>
#include <time.h>
#include "parse_rule.h"
#include "tools.h"

error execute_node(struct Node_rule* n, struct Node_var **vars);
error exec_list(char **rules, struct Node_rule **nodes, \
                struct Node_var **vars, char *parent);

#endif
