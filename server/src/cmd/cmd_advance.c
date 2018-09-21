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
** this command [advance], depends on the direction, toward the direction and
** -> move foward one cell in the map.
** MACRO for the direction are
** ->  NORTH: 0, EAST: 1, SOUTH: 2, WEST: 3
** after moving foward one step, the current player might go over the map,
**     in that case, we need to update to pos
** for example:
**     0 1 2  -> if the player is at pos 1, facing NORTH, call advance command
**     3 4 5         the update position will be at pos 7
**     6 7 8 -> if pos 8, facing EAST, update positon will be at pos 6
*/

#include "../../include/server.h"

void		cmd_advance(int fd, char *msg)
{
	int	d;

	(void)msg;
	printf(CYAN"\n[Exec ADVANCE]\n"RESET);
	printf(BLUE"Player [%d] -> [%s]"RESET, fd, "advance");
	printf("players %d, pos-> y: %d x: %d d: %d\n",
			fd, g_players[fd].y, g_players[fd].x, g_players[fd].direction);
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
	printf("players %d, pos-> y: %d x: %d d: %d\n",
			fd, g_players[fd].y, g_players[fd].x, g_players[fd].direction);
	printf(CYAN"\n[ADVANCE SUCCESS]\n"RESET);
	send_data(fd, RED"ADVANCE OK"RESET, MSG_SIZE);
}
