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

#include "../../inc/server.h"

int		cmd_advance(int fd, char *msg)
{
	int	d;

	(void)msg;
	printf(CYAN"\n[Exec ADVANCE]\n"RESET);
	printf(BLUE"Player [%d] -> [%s]"RESET, fd, "advance");
	printf("players %d, pos-> y: %d x: %d d: %d\n", fd, g_players[fd].y, g_players[fd].x, g_players[fd].direction);
	d = g_players[fd].direction;
	if (d == NORTH)
		g_players[fd].y -= 1;
	else if (d == EAST)
		g_players[fd].x += 1;
	else if (d == SOUTH)
		g_players[fd].y += 1;
	else if (d == WEST)
		g_players[fd].x -= 1;
	update_player_pos(fd);
	g_players[fd].request_nb--;
	printf("players %d, pos-> y: %d x: %d d: %d\n", fd, g_players[fd].y, g_players[fd].x, g_players[fd].direction);
	printf(CYAN"\n[ADVANCE SUCCESS]\n"RESET);
	if (send_msg(fd, RED"OK\n"RESET, "Send [advance]") == EXIT_FAILURE)
		return (EXIT_FAILURE);
	// update grahpic client regarding player position
	return (EXIT_SUCCESS);
}
