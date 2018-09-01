/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_put.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sding <sding@student.42.us.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/17 18:26:09 by sding             #+#    #+#             */
/*   Updated: 2018/08/17 18:26:09 by sding            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** 
*/

#include <server.h>

int     cmd_put(t_players players, char *msg)
{
    int res_i;

    printf(BLUE"Player [$d] -> [%s %s]\n"RESET, players.fd, "put", msg);
    players.request_nb--;
    if ((res_i = check_resource(msg)) == 7) // i think this can be handle in parse
        if (send(players.fd, "KO", 2, 0) == -1) // but here in case
            perror("Send [put]");
    if (players.inventory[res_i] == 0)
        if (send(players.fd, "KO", 2, 0) == -1)
            perror("Send [put]");
    else
    {
        players.inventory[res_i]--;
        g_env.map.cell[players.y][players.x].resource[res_i]++;
    }
    if (send(players.fd, "OK", 2, 0) == -1)
        perror("Send [put]");
    // update graphic client regarding player position
    return (EXIT_SUCCESS);
}

