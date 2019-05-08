#include "parse.h"
#include "parse_rule.h"

void parse_makefile(FILE *input)
{
    char *line = NULL;
    size_t len  = 0;
    ssize_t read;
    
    while((read = getline(&line, &len, input)) != -1)
    {
        if((strchr(line, ':')) != NULL)
        {
            struct Node_rule *tmp = create_node_rule(line, input);
            dump_node_rule(tmp);
            free_node_rule(tmp);
        }
    }
    if(line)
        free(line);
    fclose(input);
}

void parse_error(char *err, char *msg)
{
    fprintf(stderr, "Error in %s, %s\n", err, msg);
    exit(1);
}
