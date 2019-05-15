#include "parse_var.h"

char *remove_front_ws(char *string)
{
    int i = 0;
    for(; string[i]; i++)
    {
        if(string[i] == ' ' || string[i] == '\t')
            continue;
        break;
    }
    return string+i;
}

struct Node_var *create_node_var(char *line)
{
    if(*line == '=')
    {
        parse_error("Variable", "Empty variable name");
        return NULL;
    }
    char *lhs= strtok(line, "=");

    lhs = remove_front_ws(lhs);
    lhs = remove_back_ws(lhs);
    char *rhs= strtok(NULL, "=");
    rhs = remove_front_ws(rhs);

    if(strchr(lhs, ' ') != NULL)
    {
        parse_error("Variable", "Variable name split");
        return NULL;
    }
    char *name= (char *)calloc(50,sizeof(char));
    strcpy(name, lhs);

    char *value= (char *)calloc(255,sizeof(char));
    strcpy(value, rhs);

    value[strlen(value)-1]='\0';

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

struct Node_var *find_node_var(char *name, struct Node_var **vars)
{
    if(!vars)
        return NULL;
    for(int i = 0 ; vars[i] != NULL; i++)
    {
        if(vars[i])
        {
            if(strcmp(vars[i]->name,name) == 0)
                return vars[i];
        }
        return NULL;
    }
    return NULL;
}
