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

static void extract_var(char *res, char delim, char *string, int start)
{
    for(int i = start; string[i] != delim && string[i+1]; i++)
    {
        res[i] = string[i];
    }
}

static void find_and_replace(char *res, char *string, struct Node_var **vars,\
                                struct Node_var **env)
{
    int i =0;
    if(string[0] == '@')
        i++;
    int count = 0;
    int spaces = 3;
    char variable[255] = "";
    for(; string[i] != '\0'; i++)
    {
        if(string[i] == '$')
        {
            if(string[i+1] == '(')
                extract_var(variable,')',string+i+2, 0);
            else if(string[i+1] == '{')
                extract_var(variable,'}',string+i+2, 0);
            else
            {
                spaces = 2;
                variable[0] = string[i+1];
            }
            struct  Node_var *tmp = find_node_var(variable, vars);
            if(!tmp)
                tmp = find_node_var(variable, env);
            if(tmp)
            {
                if(strchr(tmp->value, '$') != NULL)
                {
                    char intermed[255] = "";
                    find_and_replace(intermed, tmp->value, vars, env);
                    strcat(res, intermed);
                    count += strlen(intermed);
                }
                else
                {
                    strcat(res, tmp->value);
                    count += strlen(tmp->value);
                }
            }
            else
            {
                strcat(res, "");
            }
            i += strlen(variable)+spaces-1;
            empty_string(variable);
        }
        else
        {
            res[count] = string[i];
            count++;
        }
    }
}

error execute_node(struct Node_rule *n, struct Node_var **vars, struct Node_var **env)
{
    vars = vars;
    char **commands = n->recipe;
    if(commands[0] == NULL)
    {
        return NoCommand;
    }
    for(int i = 0; *(commands+i) != NULL; i++)
    {
        char *res = (char *)calloc(255, sizeof(char));
        if(commands[i] != NULL)
        {
            commands[i] = strip_ws(commands[i]);
            find_and_replace(res, commands[i], vars, env);
            printf(res);
            fflush(stdout);

            pid_t child_pid;
            int status;

            if((child_pid = fork()) < 0 )
            {
                free(res);
                return ExecErrorFork;
            }
            if(child_pid == 0)
            {
                execl("/bin/sh", "sh", "-c", res, 0);
                free(res);
                return ExecErrorExecl;
            }
            else
            {
                wait(&status);
                if(WEXITSTATUS(status) != 0)
                {
                    free(res);
                    return ExecError;
                }
            }
        }
        free(res);
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

static enum error exec_rule(struct Node_rule *rule, struct Node_rule **nodes,\
        struct Node_var **vars, struct Node_var **env)
{
    int returnval = 0;
    if(strcmp(rule->depend[0], "") != 0)
        returnval = handler(returnval, exec_list(rule->depend, nodes, \
                    vars, env, rule->target));
    if(returnval > 2)
        return returnval;
    if(last_modif(rule->target) == 0)
        return handler(returnval,execute_node(rule, vars, env));
    return returnval;
}

error exec_list(char **rules, struct Node_rule **nodes, \
        struct Node_var **vars, struct Node_var **env, char *parent)
{
    int returnval = 0;
    if(!rules[0])
    {
        if(parent == NULL)
            parent = nodes[0]->target;
        return handler(returnval,exec_rule(nodes[0], nodes, vars, env));
    }
    int exist = 0;
    for(int i =0 ; *(i+rules) != NULL && (returnval <= 2 ); i++)
    {
        if(parent == NULL)
            parent = rules[i];
        if(last_modif(rules[i]) >= last_modif(parent) || last_modif(rules[i]) == 0)
        {
            if(!rule_exist(rules, rules[i], i))
            {
                if(strcmp(rules[i], "") != 0)
                {
                    struct Node_rule *tmp = find_node(rules[i], nodes);
                    if(tmp)
                    {
                        returnval = handler(returnval,exec_rule(tmp, nodes, vars, env));
                    }
                    else if (file_exist(rules[i]))
                        continue;
                    else
                    {
                        fprintf(stderr,"minimake: no rule to make target '%s'\n",rules[i]);
                        return NoRule;
                    }
                }
            }
            else
                exist = 1;
        }
        if(exist || returnval == 0)
        {
            fprintf(stdout, "minimake: '%s' is up to date.\n", rules[i]);
            returnval = 1;
        }
        if(returnval == NoCommand && parent == rules[i])
        {
            fprintf(stdout, "minimake: Nothing to be done for '%s'.\n", parent);
        }
    }
    return returnval;
}
