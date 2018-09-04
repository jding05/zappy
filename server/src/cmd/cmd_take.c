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

#include "../../inc/server.h"

int     cmd_take(t_players players, char *msg)
{
    int res_i;

    printf(BLUE"Player [%d] -> [%s %s]"RESET, players.fd, "take", msg);
    players.request_nb--;
    if ((res_i = check_resource(msg)) == 7) // i think this can be handle in parse
	{
		if (send_msg(players.fd, "KO", "Send [take]") == EXIT_FAILURE)
			return (EXIT_FAILURE);
	}
    if (g_env.map[players.y][players.x][res_i] == 0)
	{
		if (send_msg(players.fd, "KO", "Send [take]") == EXIT_FAILURE)
			return (EXIT_FAILURE);
	}
    else
    {
        g_env.map[players.y][players.x][res_i]--;
        players.inventory[res_i]++;
    }
	if (send_msg(players.fd, "OK", "Send [take]") == EXIT_FAILURE)
		return (EXIT_FAILURE);
    // update graphic client regarding player position
    return (EXIT_SUCCESS);
}

int     check_resource(char *msg)
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
