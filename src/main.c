#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "options.h"
#include "parse.h"
#include "parse_rule.h"
#include "execute_node.h"
#include "tools.h"

int main (int argc, char *argv[])
{
    char **rules;
    struct Node_rule **nodes;
    if(find_option(argv, argc, 'h') != -1)
    {
        dump_help();
        return 0;
    }
    FILE *file = NULL;
    int index = find_option(argv, argc, 'f');
    if(index != -1)
    {
        file = handle_file(argv[index+1], 1);
        if(file == NULL)
            return 1;
    }
    else
    {
        file = handle_file("makefile", 0);
        if(file == NULL)
        {
            file = handle_file("Makefile", 1);
            if(file == NULL)
                return 1;
        }
    }
    rules = find_rules(argv, argc-1);
    if(file != NULL)
        nodes = parse_makefile(file);
    if(!nodes[0])
    {
        fprintf(stderr, "minimake: no target to build");
        exit(2);
    }
    enum error returnval = exec_list( rules, nodes);
    handle_return_and_free(returnval, rules, nodes);
    return 0;
}
