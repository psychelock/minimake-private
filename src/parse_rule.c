#include "parse_rule.h"

static int words(const char *sentence)
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

static void move_depend(char **tmp, int i)
{
    if(!tmp)
        return;
    for(; tmp[i+1]; i++)
    {
        strcpy(tmp[i], tmp[i+1]);
    }
    empty_string(tmp[i]);
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

static int justws(char *buffer)
{
    for(size_t i =0; i < strlen(buffer); i++)
    {
        if(buffer[i]=='\t' || buffer[i]== ' ' || buffer[i] == '\n')
            continue;
        return 0;

    }
    return 1;
}

static int comment(char *buffer)
{
    for(size_t i = 0; i < strlen(buffer); i++)
    {
        if(buffer[i]=='\t' || buffer[i]==' ' || buffer[i] =='\n')
            continue;
        if(buffer[i] == '#')
            return 1;
        else
            return 0;
    }
    return 0;
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
        if(buffer[0] == '\t')
        {
            if(justws(buffer))
                continue;
            if(handle_var_error(buffer))
            {
                parse_error("Variable", "Syntax error");
                free_string(res);
                free(buffer);
                return NULL;
            }
            res[count-1] = (char*)malloc(255*sizeof(char));
            strcpy(res[count-1], buffer);
            count++;
            res = realloc(res, (count) * sizeof(char*));
            res[count-1] = NULL;
        }
        else if(comment(buffer))
            continue;
        else
        {
            disgusting(buffer, input);
            break;
        }
    }
    free(buffer);
    return res;
}

char *remove_back_ws(char *string)
{
    int len = strlen(string)-1;
    for(int i = len;i >=0; i--)
    {
        if(string[i] == '\t' || string[i] == ' ')
            string[i] = '\0';
        else
            break;
    }
    return string;
}

static void extract_var(char *res, char delim, char *string, int start)
{
    for(int i = start; string[i] != delim && string[i+1]; i++)
    {
        res[i] = string[i];
    }
}

static void find_and_replace(char *dest, char *string, struct Node_var **vars)
{
    int spaces = 3;
    char variable[255] = "\0";
    if(*(string+1) == '(')
        extract_var(variable, ')', string+2, 0);
    else if(*(string+1) == '{')
        extract_var(variable, '}', string+2, 0);
    else
    {
        spaces = 2;
        variable[0] = *(string+1);
    }
    struct Node_var *tmp = find_node_var(variable,vars);
    if(tmp)
    {
        spaces += strlen(variable);
        strcat(dest,tmp->value);
        strcat(dest, string+spaces);
    }
    else
    {
        strcat(dest, string+strlen(variable)+spaces );
    }
}


static void change_var_depend(char **depend, struct Node_var **vars)
{
    char variable[255] = "";
    if(!depend)
        return;
    for(int i =0; depend[i]; i++)
    {
        if(strchr(depend[i], '$') != NULL)
        {
            if(*(depend[i]+1) == '(')
                extract_var(variable, ')', depend[i]+2, 0);
            else if(*(depend[i]+1) == '{')
                extract_var(variable, '}', depend[i]+2, 0);
            else
                variable[0] = *(depend[i]+1);
            struct Node_var* tmp = find_node_var(variable, vars);
            if(tmp)
                strcpy(depend[i], tmp->value);
            else
            {
                move_depend(depend, i);
                i--;
            }
            empty_string(variable);
        }
    }
}

struct Node_rule* create_node_rule (char *line1, FILE *input, struct Node_var ** vars)
{
    if(*line1 == ':')
    {
        parse_error("Target", "Empty target");
        return NULL;
    }

    char *target= remove_front_ws(remove_back_ws(strtok(line1, ":")));

    if(strchr(target, ' ') != NULL)
    {
        parse_error("Target", "Multiple targets");
        return NULL;
    }

    char *rule = (char *)calloc(50,sizeof(char));

    char *replacement = strchr(target, '$');
    if(replacement)
        find_and_replace(rule, replacement, vars);
    else
        strcpy(rule, target);

    char *depend= strtok(NULL, ":");
    char **alldepend = split_depend(depend);
    change_var_depend(alldepend, vars);
    char **recipe = split_commands(input);

    if(!recipe)
    {
        free(target);
        return NULL;
    }

    struct Node_rule *res = (struct Node_rule*)malloc(sizeof(struct Node_rule));
    char *implicittarget = (char *)malloc(50 * sizeof(char));
    char **implicitdepend = (char **)malloc(sizeof(char *));
    implicitdepend[0]= NULL;
    res->target = rule;
    res->depend = alldepend;
    res->recipe = recipe;
    res->implicittarget = implicittarget;
    res->implicitdepend = implicitdepend;
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
    free(n->implicittarget);
    free_string(n->implicitdepend);
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

static int wildcard(const char *pattern, const char *string)
{
    if(*pattern == '\0')
        return (*string =='\0');
    if(*pattern == *string)
        return (*string != '\0' && wildcard(pattern+1, string +1));
    if(*pattern == '%')
        return (wildcard(pattern+1,string) || \
                (*string != '\0' && wildcard(pattern, string+1)));
    return 0;
}


struct Node_rule *find_node(char *target, struct Node_rule **nodes)
{
    struct Node_rule *res = NULL;
    size_t len = 0;
    if(!nodes)
        return NULL;
    for(int i = 0; nodes[i] != NULL; i++)
    {
        if(wildcard(nodes[i]->target, target))
        {
            if(len <= strlen(nodes[i]->target))
            {
                len = strlen(nodes[i]->target);
                res = nodes[i];
            }
        }
    }
    return res;
}
