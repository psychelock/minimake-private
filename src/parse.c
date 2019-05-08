#include "parse.h"
#include "parse_rule.h"

void parse_makefile(FILE *input)
{
    char *line1 = NULL;
    size_t len1  = 0;
    ssize_t read1;
    
    char *line2 = NULL;
    size_t len2 = 0;
    ssize_t read2;

    char *pos = NULL;
    while((read1 = getline(&line1, &len1, input)) != -1)
    {
        if((pos = strchr(line1, ':')) != NULL)
        {
            if((read2 = getline(&line2, &len2, input)) != -1)
            {
                struct Node_rule *tmp = create_node_rule(line1, line2);
                dump_node_rule(tmp);
                free_node_rule(tmp);
            }
        }
    }
    if(line1 != NULL)
        free(line1);
    if(line2 != NULL)
        free(line2);
    fclose(input);
}

void parse_error(char *err, char *msg)
{
    fprintf(stderr, "Error in %s, %s\n", err, msg);
    exit(1);
}
