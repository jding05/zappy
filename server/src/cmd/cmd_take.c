/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_take.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sding <sding@student.42.us.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/17 18:25:54 by sding             #+#    #+#             */
/*   Updated: 2018/08/17 18:25:55 by sding            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** take an object
*/

#include <server.h>

int     cmd_take(t_players players, char *msg)
{
    int res_i;

    printf(BLUE"Player [$d] -> [%s %s]"RESET, players.fd, "take", msg);
    players.request_nb--;
    if ((res_i = check_resource(msg)) == 7) // i think this can be handle in parse
        if (send(players.fd, "KO", 2, 0) == -1) // but here in case
            perror("Send [take]");
        //    return (EXIT_FAILURE);
    if (g_env.map.cell[players.y][players.x].resource[res_i] = 0)
        if (send(players.fd, "KO", 2, 0) == -1)
            perror("Send [take]");
        //    return (EXIT_FAILURE);
    else
    {
        if (g_env.map.cell[players.y][players.x].resource[res_i] == 0)
            if (send(players.fd, "KO", 2, 0) == -1)
                perror("Send [take]");
            //    return (EXIT_FAILURE);
        else
            g_env.map.cell[players.y][players.x].resource[res_i]--;
        players.inventory[res_i]++;
    }
    if (send(players.fd, "OK", 2, 0) == -1)
        perror("Send [take]");
        //    return (EXIT_FAILURE);
    // update graphic client regarding player position
    return (EXIT_SUCCESS);
}

int     check_resource(msg)
{
    if (strcmp("food", msg) == 0)
        return (0);
    else if (strcmp("linemate", msg) == 0)
        return (1);
    else if (strcmp("deraumere", msg) == 0)
        return (2);
    else if (strcmp("sibur", msg) == 0)
        return (3);
    else if (strcmp("mendiane", msg) == 0)
        return (4);
    else if (strcmp("phiras", msg) == 0)
        return (5);
    else if (strcmp("thystame", msg) == 0)
        return (6);
    else
        return (7);
}