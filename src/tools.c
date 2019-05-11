#include "tools.h"

int rule_exist(char **rules, char *find, int n)
{
    if(!rules)
        return 0;
    for(int i = 0; *(rules+i) != NULL && i < n; i++)
    {
        if(strcmp(rules[i],find) == 0)
            return 1;
    }
    return 0;
}

char **find_rules (char *list[], int n)
{
    int count = 0;
    char **res = (char**)malloc((n+1)* sizeof(char*));
    for(int i = 0; i < n; i++)
    {
        res[i] = NULL;
    }
    int index = find_option(list, n, 'f');
    for(int i = 1; i <= n; i++)
    {
        if(i != index+1 && *list[i] != '-')
        {
            res[count] = (char*)malloc(50*sizeof(char));
            strcpy(res[count], list[i]);
            count++;
        }
    }
    res[count] = NULL;
    return res;
}

void dump_rules(char **rules)
{
    if(rules == NULL)
        return;
    for(int i = 0; *(rules+i) != NULL; i++)
    {
        if(rules[i] != NULL)
        {
            printf(rules[i]);
            printf(" ");
        }
    }
}

void free_string(char **rules)
{
    if(rules == NULL)
        return;
    for(int i = 0; *(rules + i) != NULL; i++)
    {
        if(rules[i] != NULL)
        {
            free(rules[i]);
        }
    }
    free(rules);
}

int last_modif(char *path)
{
    struct stat b;
    if (!stat(path, &b))
    {
        return (int)mktime(localtime(&b.st_mtime));
    }
    else
        return 0;
}

void free_resources(char **rules, struct Node_rule **nodes)
{
    free_all_nodes(nodes);
    free_string(rules);
}

void handle_return_and_free(enum error val, char **rules, struct Node_rule **nodes)
{
    free_resources(rules, nodes);
    switch(val)
    {
        case ExecErrorFork:
        {
            fprintf(stderr, "exec fails at fork\n");
            exit(2);
        }
        case ExecErrorExecl:
        {
            fprintf(stderr, "exec fails at execl\n");
            exit(2);
        }
        case ExecError:
        {
            fprintf(stderr, "exec fails\n");
            exit(2);
        }
        case NoRule:
        {
            exit(2);
        }
        case NoError:
        {
            exit(0);
        }
        default:
        {
            fprintf(stderr, "Not possible");
            exit(3);
        }
    }
}
