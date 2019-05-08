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
        //res = realloc(res, (count+2 * sizeof(char*)));

        res[count] = (char*)malloc(50*sizeof(char));
        strcpy(res[count], token);
        count++;
        token = strtok(NULL, " ");
    }
    res[count] = NULL;
    return res;
}

struct Node_rule* create_node_rule (char *line1, char *line2)
{
    if(*line1 == ':')
        parse_error("Target", "Empty target");
    char *target= strtok(line1, ":");
    if(strchr(target, ' ') != NULL)
        parse_error("Target", "Multiple targets");
    char *depend= strtok(NULL, ":");
    if(*line2 != 9 && *line2 != '\n')
        parse_error("Recipe", "Missing beginning tab");
    char *recipe= line2;

    struct Node_rule *res = (struct Node_rule*)malloc(sizeof(struct Node_rule));
    res->target = target;
    res->depend = split_depend(depend);
    res->recipe = recipe;
    return res;
}

void dump_node_rule(struct Node_rule* n)
{
    printf("Target is           :%s\n",n->target);
    printf("Dependenc are       :");
    dump_rules(n->depend);
    printf("Recipe is           :%s\n",n->recipe);
}

void free_node_rule(struct Node_rule *n)
{
    free_string(n->depend);
    free(n);
}
