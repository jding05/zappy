#include "client.h"
#include <stdio.h>

void    test_print_flags(t_env *env)
{
    printf("-n %s\t-p %d\t-h %s\n", env->team_name, env->port, env->hostname);
}

int     main(int argc, char **argv)
{
    t_env env;

    if (!read_flags(argc, argv, &env))
        return (print_usage());

    init map, events, seed board, and establish users in queue
    
    init connection to all users

    while(game still valid and or connections still going)
    {
        parse all events and put into queue
        while (there is a queue)
        {

        }
        collect data to broadcast and send out of prospective users
    }

    return (0);
}