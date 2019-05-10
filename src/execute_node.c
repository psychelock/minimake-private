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

void execute_node(struct Node_rule *n)
{
    int dontprint;
    char **commands = n->recipe;
    if(commands[0] == NULL)
    {
        printf("minimake: Nothing to be done for '%s'.\n", n->target);
        return;
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
                perror("fork failure");
                fprintf(stderr, "Fork failure\n");
                exit(1);
            }
            if(child_pid == 0)
            {
                execl("/bin/sh", "sh", "-c", commands[i], 0);
                fprintf(stderr, "execl failure!\n");
                exit(2);
            }
            else
            {
                if(dontprint)
                    commands[i] = add_start(commands[i], '@');
                wait(&status);
                if(WEXITSTATUS(status) != 0)
                    exit(2);
            }
        }
    }
}

static struct Node_rule *find_node(char *target, struct Node_rule **nodes)
{
    if(!nodes)
        return NULL;
    for(int i = 0; *(i + nodes) != NULL; i++)
    {
        if(strcmp(nodes[i]->target,target) ==0)
            return nodes[i];
    }
    return NULL;
}
void execute_all_dep(char *target, char **depend, struct Node_rule **nodes
                       ,int parent)
{
    //char targettime[100] = "";
    struct tm *tm = last_modif(target);
    //strftime(targettime, 100, "%d/%m/%Y %H:%M:%S", targettm);
    if(tm && parent == 1)
    {
        printf("minimake: Nothing to be done for '%s'.\n", target);
        return;
    }
    else if(tm && ( strcmp(depend[0], "\n") == 0))
    {
        return;
    }
    else
    {
        exec_all_rules(depend, nodes);
    }
}

void exec_all_rules(char **rules, struct Node_rule **nodes)
{
    if(!rules[0])
    {
        execute_all_dep(nodes[0]->target , nodes[0]->depend, nodes, 1);
        execute_node(nodes[0]);
        return;
    }
    for(int i =0 ; *(i+rules) != NULL; i++)
    {
        struct Node_rule *tmp = find_node(rules[i], nodes);
        if(tmp)
        {
            execute_all_dep(tmp->target , tmp->depend, nodes, 1);
            execute_node(tmp);
        }
        else
        {
            printf("minimake: no rule to make target '%s'\n", rules[i]);
            exit(1);
        }
    }
}


