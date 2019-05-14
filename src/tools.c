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
    if(!rules)
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

int file_exist(char *path)
{
    FILE *file;
    if((file = fopen(path, "r")))
    {
        fclose(file);
        return 1;
    }
    return 0;
}

void free_resources(char **rules, struct AllNodes *allnodes)
{
    free_all_node_var(allnodes->vars);
    free_all_nodes(allnodes->nodes);
    free(allnodes);
    free_string(rules);
}

enum error handler(enum error val1, enum error val2)
{
    if(val1 > 2)
        return val1;
    else if(val2 > 2)
        return val2;
    else if(val1 == 0)
        return val2;
    else if(val2 == 0)
        return val1;
    else if(val1 == 2 && val2 == 2)
        return NoCommand;
    else
        return NoError;
}

void handle_return(enum error val)
{
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
        case NoCommand:
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

int handle_var_error(char *line)
{
    int startbra = 0;
    int startcurly = 0;
    size_t len = strlen(line);
    for(size_t i = 0; i < len-1; i++)
    {
        if(line[i] == '$')
        {
            if (line[i+1] == '(' && startbra == 0)
                startbra = 1;
            else if (line[i+1] == '{' && startcurly == 0)
                startcurly = 1;
        }
        if(startbra == 1 && line[i] == ')')
            startbra = 0;
        if(startcurly == 1 && line[i] == '}')
            startcurly = 0;
    }
    return (startbra || startcurly);
}
