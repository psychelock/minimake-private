#include "options.h"

int find_option(char *list[], int n, char opt)
{
    char option[2];
    option[0] = '-';
    option[1] = opt;
    for(int i = 1; i < n && list[i] != NULL; i ++)
    {
        if (strncmp(list[i],option, 2) == 0)
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


