#ifndef OPTIONS_H
#define OPTIONS_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <memory.h>

int find_option(char *list[], int n, char opt);
void dump_help();
FILE *handle_file(char *filename, int option);
#endif
