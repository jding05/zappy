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
** check_resource function is in cmd_take
*/

#include <server.h>

int		cmd_put(t_players players, char *msg)
{
	int res_i;

	printf(BLUE"Player [$d] -> [%s %s]\n"RESET, players.fd, "put", msg);
	players.request_nb--;
	if ((res_i = check_resource(msg)) == 7) // i think this can be handle in parse
	{
		if (send_msg(players.fd, "KO", "Send [put]") == EXIT_FAILURE)
			return (EXIT_FAILURE);
	}
	if (players.inventory[res_i] == 0)
	{
		if (send_msg(players.fd, "KO", "Send [put]") == EXIT_FAILURE)
			return (EXIT_FAILURE);
	}
	else
	{
		players.inventory[res_i]--;
		g_env.map[players.y][players.x][res_i]++;
	}
	if (send_msg(players.fd, "OK", "Send [put]") == EXIT_FAILURE)
		return (EXIT_FAILURE);
	// update graphic client regarding player position
	return (EXIT_SUCCESS);
}

int		send_msg(int fd, char *status, char *error_msg)
{
	if (send(fd, status, strlen(status), 0) == -1)
	{
		perror(error_msg);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}