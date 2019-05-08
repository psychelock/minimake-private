#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "options.h"
#include "parse.h"

int main (int argc, char *argv[])
{
    
    if(find_option(argv, argc, 'h') != -1)
    {
        dump_help();
        return 0;
    }
    FILE *file = NULL;
    int index = find_option(argv, argc, 'f');
    if(index != -1)
    {
        file = handle_file(argv[index+1], 1);
        if(file == NULL)
            return 1;
    }
    else
    {
        file = handle_file("makefile", 0);
        if(file == NULL)
        {
            file = handle_file("Makefile", 1);
            if(file == NULL)
                return 1;
        }
    }
    char **rules = find_rules(argv, argc-1);
    //dump_rules(rules);
    free_string(rules);
    if(file != NULL)
        parse_makefile(file);
}
