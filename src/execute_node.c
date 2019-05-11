#include "execute_node.h"

static char *strip_ws(char *a)
{
    char *res = (char *) malloc(255 *sizeof(char));
    size_t i = 0;
    while (i< strlen(a)-1)
    {
        if (a[i]==' '|| a[i]=='\t')
        {
            i++;
        }
        else
            break;
    }
    strcpy(res, (a+i));
    if(a != NULL)
        free(a);
    return res;
}

static char *remove_start(char *a, char b)
{
    char *res = (char *) malloc (255 * sizeof(char));
    if(a[0] == b)
    {
        strcpy(res, (a+1));
        if(a != NULL)
            free(a);
        return res;
    }
    free(res);
    return a;
}

static char *add_start(char *a, char b)
{
    char *res = (char *)malloc(255 * sizeof(char));
    res[0] = b;
    strcpy(res+1, a);
    if(a)
        free (a);
    return res;
}

error execute_node(struct Node_rule *n)
{
    int dontprint;
    char **commands = n->recipe;
    if(commands[0] == NULL)
    {
        printf("minimake: Nothing to be done for '%s'.\n", n->target);
        return NoError; /*FIXME add parent */
    }
    for(int i = 0; *(commands+i) != NULL; i++)
    {
        dontprint = 0;
        if(commands[i] != NULL)
        {
            commands[i] = strip_ws(commands[i]);
            if(*commands[i] == '@')
            {
                dontprint = 1;
                commands[i] = remove_start(commands[i], '@');
            }
            if(!dontprint)
                printf(commands[i]);
            fflush(stdout);

            pid_t child_pid;
            int status;

            if((child_pid = fork()) < 0 )
            {
                return ExecErrorFork;
            }
            if(child_pid == 0)
            {
                execl("/bin/sh", "sh", "-c", commands[i], 0);
                return ExecErrorExecl;
            }
            else
            {
                if(dontprint)
                    commands[i] = add_start(commands[i], '@');
                wait(&status);
                if(WEXITSTATUS(status) != 0)
                    return ExecError;
            }
        }
    }
    return NoError;
}

static struct Node_rule *find_node(char *target, struct Node_rule **nodes)
{
    for(int i = 0; *(i + nodes) != NULL; i++)
    {
        if(strcmp(nodes[i]->target,target) ==0)
            return nodes[i];
    }
    return NULL;
}

static enum error exec_rule(struct Node_rule *rule, struct Node_rule **nodes)
{
    int returnval = 0;
    if(strcmp(rule->depend[0], "") != 0)
        returnval = exec_list(rule->depend, nodes);
    if(returnval != 0)
        return returnval;
    return execute_node(rule);
}

error exec_list(char **rules, struct Node_rule **nodes)
{
    int returnval = 0;
    if(!rules[0])
    {
        return exec_rule(nodes[0], nodes);
    }
    for(int i =0 ; *(i+rules) != NULL && returnval == NoError; i++)
    {
        if(!rule_exist(rules, rules[i], i))
        {
            struct Node_rule *tmp = find_node(rules[i], nodes);
            if(tmp)
            {
                returnval = exec_rule(tmp, nodes);
            }
            else
            {
                fprintf(stderr,"minimake: no rule to make target '%s'\n",rules[i]);
                return NoRule;
            }
        }
        else
        {
            printf("minimake: '%s' is up to date.\n", rules[i]);
        }
    }
    return returnval;
}
