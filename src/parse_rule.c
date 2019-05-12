#include "parse_rule.h"

int words(const char *sentence)
{
    int count=0,i,len;
    char lastC;
    len=strlen(sentence);
    if(len > 0)
    {
        lastC = sentence[0];
    }
    for(i=0; i<=len; i++)
    {
        if((sentence[i]==' ' || sentence[i]=='\0') && lastC != ' ')
        {
            count++;
        }
        lastC = sentence[i];
    }
    return count;
}

static char **split_depend(char *depend)
{
    int num = words(depend)+1;
    int count = 0;
    char **res = (char**)malloc(num * sizeof(char*));
    char *token = strtok(depend, " ");
    while(token != NULL)
    {
        res[count] = (char*)malloc(50*sizeof(char));
        strcpy(res[count], token);
        count++;
        token = strtok(NULL, " ");
    }
    size_t len = strlen(res[count-1])-1;
    res[count-1][len] = '\0';            /* removing last \n */
    res[count] = NULL;
    return res;
}

static void disgusting(char *string, FILE *input)
{
    for(int i = strlen(string)-1; i >=0; i--)
    {
        ungetc(string[i], input);
    }
}

static char **split_commands(FILE *input)
{
    int count = 1;
    char **res = (char**)malloc(sizeof(char*));
    res[0]=NULL;
    char *buffer= NULL;
    size_t len  = 0;
    ssize_t read;

    while((read = getline(&buffer, &len, input)) != -1)
    {
        if(buffer[0] == 9 || buffer[0] == '#' || buffer[0] == '\n')
        {
            if(buffer[0] == '\n')
                continue;
            res[count-1] = (char*)malloc(255*sizeof(char));
            strcpy(res[count-1], buffer);
            count++;
            res = realloc(res, (count) * sizeof(char*));
            res[count-1] = NULL;
        }
        else
        {
            disgusting(buffer, input);
            break;
        }
    }
    free(buffer);
    return res;
}

struct Node_rule* create_node_rule (char *line1, FILE *input)
{
    if(*line1 == ':')
        parse_error("Target", "Empty target");
    char *target= strtok(line1, ":");
    char *rule = (char *)malloc(sizeof(char *));
    strcpy(rule, target);

    if(strchr(target, ' ') != NULL)
        parse_error("Target", "Multiple targets");
    char *depend= strtok(NULL, ":");

    char **recipe = split_commands(input);

    struct Node_rule *res = (struct Node_rule*)malloc(sizeof(struct Node_rule));
    res->target = rule;
    res->depend = split_depend(depend);
    res->recipe = recipe;
    return res;
}

void dump_node_rule(struct Node_rule* n)
{
    printf("Target is           :%s\n\n",n->target);
    printf("Dependencies are    :");
    dump_rules(n->depend);
    printf("\n");
    printf("Recipes are         :\n");
    dump_rules(n->recipe);
}

void free_node_rule(struct Node_rule *n)
{
    free(n->target);
    free_string(n->depend);
    free_string(n->recipe);
    free(n);
}

void free_all_nodes(struct Node_rule **n)
{
    if(!n)
        return;
    for(int i = 0; *(n + i) != NULL; i++)
    {
        if(n[i] != NULL)
        {
            free_node_rule(n[i]);
        }
    }
    free(n);
}
