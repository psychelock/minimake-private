#include <stdio.h>
#include <string.h>
#include "options.h"

int main (int argc, char *argv[])
{
    if(find_option(argv, argc, 'h') != -1)
    {
        dump_help();
        return 0;
    }
    int index = find_option(argv, argc, 'f');
    if(index != -1)
    {
        if(handle_file(argv[index+1], 1) == -1)
            return 1;
    }
    else
    {
        handle_file("makefile", 0);
        handle_file("Makefile", 1);
    }
    char **rules = find_rules(argv, argc-1);
    dump_rules(rules, argc -1);
    free_string(rules, argc -1);
}
