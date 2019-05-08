#include "options.h"

static int compare(char *first, char *second)
{
    while (*first == *second) 
    {
        if (*first == '\0' || *second == '\0')
            break;

        first++;
        second++;
    }

    if (*first == '\0' && *second == '\0')
        return 0;
    else
        return -1;
}

int find_option(char *list[], int n, char opt)
{
    char option[2];
    option[0] = '-';
    option[1] = opt;
    for(int i = 1; i < n ; i ++)
    {
        if (compare (list[i],option) == 0)
            return i;
    }
    return -1;
}

void dump_help()
{
    printf("minimake - make utility to maintain groups of programs \n");
    printf("Allowed options are \n -f FILE \n -h\n");
}

FILE *handle_file(char *filename, int option)
{
    FILE *file;
    file = fopen(filename, "r");
    if(file == NULL)
    {
        if(option == 1)
            printf("Error: File cannot be found/opened\n");
        return NULL;
    }
    return file;
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
