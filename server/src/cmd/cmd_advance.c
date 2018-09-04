/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_advance.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sding <sding@student.42.us.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/17 18:25:17 by sding             #+#    #+#             */
/*   Updated: 2018/08/17 18:25:19 by sding            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** 
*/

#include <server.h>

int		cmd_advance(t_players players, char *msg)
{
	int	d;

	printf(BLUE"Player [$d] -> [%s]"RESET, players.fd, "advance");
	d = players.direction;
	if (d == NORTH)
		players.y -= 1;
	else if (d == EAST)
		players.x += 1;
	else if (d == SOUTH)
		players.y += 1;
	else if (d == WEST)
		players.x -= 1;
	if (players.x >= g_env.map_x)
		players.x = 0;
	else if (players.x < 0)
		players.x = g_env.map_x - 1;
	if (players.y >= g_env.map_y)
		players.y = 0;
	else if (players.y < 0)
		players.y = g_env.map_y - 1;
	players.request_nb--;
	if (send_msg(players.fd, "OK", "Send [advance]") == EXIT_FAILURE)
		return (EXIT_FAILURE);
	// update grahpic client regarding player position
	return (EXIT_SUCCESS);
}
