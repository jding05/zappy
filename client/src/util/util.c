#include "util.h"

//  Usage: ./client -n <team> -p <port> [-h <hostname>]
//      -n team\_name
//  -p port
//  -h name of the host, by default it'll be localhost
int     record_flag(int m, int i, char **argv, t_env *env)
{
    if (m == 0)
        env->team_name = strdup(argv[i + 1]);
    if (m == 1)
        env->port = atoi(argv[1 + i]);
    if (m == 2)
        env->hostname = strdup(argv[1 + i]);
    return (1);
}

int     flag_check(t_env *env)
{
    if (env->hostname == NULL)
        env->hostname = strdup(LOCALHOST);
    if (env->team_name == NULL)
    {
        printf("Please state a team neame\n");
        return (0);
    }    
    if (env->port == 0)
    {
        printf("Please state a port number\n");
        return (0);
    }
    return (1);
}

static char    *g_options[] = {"-n","-p","-h"};

int     read_flags(int argc, char **argv, t_env *env)
{
    int i;
    int m;

    i = 1;
    while (argv[i])
    {
        m = 0;
        while (m < 3)
        {
            if (!strcmp(g_options[m], argv[i]) && (i + 1 < argc))
            {
                record_flag(m, i, argv, env);
                break;
            }
            m++;
        }
        i++;
    }
    if (!flag_check(env))
        return (0);
    return (1);
}