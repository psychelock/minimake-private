#include "tools.h"

static int rule_exist(char **rules, char *find)
{
    if(!rules)
        return 0;
    for(int i = 0; *(rules+i) != NULL; i++)
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
        if(i != index+1 && *list[i] != '-' && !rule_exist(res, list[i]))
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

struct tm* last_modif(char *path)
{
    struct stat b;
    if (!stat(path, &b))
    {
        return localtime(&b.st_mtime);
    }
    else
        return NULL;
}
