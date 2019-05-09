#include "parse.h"
#include "parse_rule.h"
#include "execute_node.h"

struct Node_rule **parse_makefile(FILE *input)
{
    int node_count = 1;
    struct Node_rule **res = \
                    (struct Node_rule**) malloc (sizeof(struct Node_rule*));
    res[0] = NULL;
    char *line = NULL;
    size_t len  = 0;
    ssize_t read;
    while((read = getline(&line, &len, input)) != -1)
    {
        if((strchr(line, ':')) != NULL)
        {
            struct Node_rule *tmp = create_node_rule(line, input);
            res[node_count-1] = tmp;
            node_count++;
            res = realloc(res, (node_count) * sizeof(struct Node_rule*));
            res[node_count-1] = NULL;
        }
    }
    if(line)
        free(line);
    fclose(input);
    return res;
}

void parse_error(char *err, char *msg)
{
    fprintf(stderr, "Error in %s, %s\n", err, msg);
    exit(1);
}
