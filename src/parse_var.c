#include "parse_var.h"

struct Node_var *create_node_var(char *line)
{
    if(*line == ':')
        parse_error("Variable", "Empty variable name");
    char *lhs= strtok(line, "=");
    char *name= (char *)malloc(sizeof(char *));
    strcpy(name, lhs);

    char *rhs= strtok(NULL, "=");
    char *value= (char *)malloc(sizeof(char *));
    strcpy(value, rhs);

    struct Node_var *res = (struct Node_var*) malloc (sizeof(struct Node_var));
    res->name = name;
    res->value = value;
    return res;
}

static void free_node_var(struct Node_var *tmp)
{
    if(tmp)
    {
        if(tmp->name)
            free(tmp->name);
        if(tmp->value)
            free(tmp->value);
        free(tmp);
    }
}

void free_all_node_var(struct Node_var **vars)
{
    if(!vars)
        return;
    for(int i = 0 ; vars[i]!= NULL; i++)
    {
        free_node_var(vars[i]);
    }
    free(vars);
}
