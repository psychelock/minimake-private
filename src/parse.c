#include "parse.h"
#include "parse_rule.h"
#include "parse_var.h"
#include "execute_node.h"

struct AllNodes *parse_makefile(FILE *input)
{
    int node_count = 1;
    int var_count = 1;

    struct AllNodes *res = (struct AllNodes*) malloc (sizeof(struct AllNodes));
    struct Node_rule **nodes = \
                    (struct Node_rule**) malloc (sizeof(struct Node_rule*));
    struct Node_var **vars = \
                    (struct Node_var**) malloc(sizeof(struct Node_var*));
    nodes[0] = NULL;
    vars[0] = NULL;


    char *line = NULL;
    size_t len  = 0;
    ssize_t read;
    while((read = getline(&line, &len, input)) != -1)
    {
        if((strchr(line, ':')) != NULL)
        {
            struct Node_rule *tmp = create_node_rule(line, input);
            nodes[node_count-1] = tmp;
            node_count++;
            nodes = realloc(nodes, (node_count) * sizeof(struct Node_rule*));
            nodes[node_count-1] = NULL;
        }
        else if((strchr(line, '=')) != NULL)
        {
            struct Node_var *tmp = create_node_var(line);
            vars[var_count-1] = tmp;
            var_count++;
            vars = realloc(nodes, (var_count) * sizeof(struct Node_var*));
            vars[var_count-1] = NULL;
        }
    }
    if(line)
        free(line);
    fclose(input);
    res->nodes = nodes;
    res->vars= vars;
    return res;
}

void parse_error(char *err, char *msg)
{
    fprintf(stderr, "Error in %s, %s\n", err, msg);
    exit(1);
}
