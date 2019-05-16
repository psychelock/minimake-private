#include "parse.h"
#include "parse_rule.h"
#include "parse_var.h"
#include "execute_node.h"

struct AllNodes *parse_makefile(FILE *input)
{
    int error = 0;
    int total_node = 1;
    int total_var = 1;
    int node_count = 0;
    int var_count = 0;

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
        if(handle_var_error(line))
        {
            parse_error("Variable", "Syntax error");
            error = 1;
            break;
        }
        if((strchr(line, ':')) != NULL)
        {
            struct Node_rule *tmp_rule = create_node_rule(line, input, vars);
            if(!tmp_rule)
            {
                error =1;
                break;
            }
            nodes[node_count] = tmp_rule;
            node_count++;
            if(node_count >= total_node)
            {
                total_node *= 2;
                nodes = realloc(nodes, (total_node) * sizeof(struct Node_rule*));
            }
            nodes[node_count] = NULL;
        }
        else if((strchr(line, '=')) != NULL)
        {
            struct Node_var *tmp_var = create_node_var(line, 0);
            if(!tmp_var)
            {
                error = 1;
                break;
            }
            vars[var_count] = tmp_var;
            var_count++;
            if(var_count >= total_var)
            {
                total_var *= 2;
                vars = realloc(vars, (var_count) * sizeof(struct Node_var*));
            }
            vars[var_count] = NULL;
        }
        else
        {
            if(strcmp(line, "\n") != 0)
            {
                error = 1;
                parse_error("File", "Missing seperator");
                break;
            }
        }
        res->nodes = nodes;
        res->vars= vars;
    }
    if(line)
        free(line);
    fclose(input);
    res->nodes = nodes;
    res->vars= vars;
    res->error = error;
    return res;
}

void parse_error(char *err, char *msg)
{
    fprintf(stderr, "Error in %s, %s\n", err, msg);
}
