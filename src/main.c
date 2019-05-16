#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "options.h"
#include "parse.h"
#include "parse_rule.h"
#include "execute_node.h"
#include "tools.h"

int main (int argc, char *argv[]/*,char *envp[]*/)
{
    char **rules;
    struct AllNodes *allnodes;
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
        allnodes = parse_makefile(file);
    if(allnodes->error)
    {
        free_resources(rules, allnodes);
        exit(2);
    }

    if(!find_non_implicit_node(allnodes->nodes))
    {
        fprintf(stderr, "minimake: no target to build\n");
        free_resources(rules, allnodes);
        exit(2);
    }
    struct Node_var **envvar =/* parse_env_var(envp);*/NULL;

    enum error returnval = exec_list( rules, allnodes->nodes,\
                                        allnodes->vars, envvar, NULL);
    //envvar = envvar;
    free_resources(rules, allnodes);
    free_all_node_var(envvar);
    handle_return(returnval);
    return 0;
}
