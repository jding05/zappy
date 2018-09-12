/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_left.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sding <sding@student.42.us.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/17 18:25:33 by sding             #+#    #+#             */
/*   Updated: 2018/08/17 18:25:35 by sding            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** trun left 90 degrees
** 0:N , 1:E, 2:S, 3:W
*/
#include "../../inc/server.h"

int		cmd_left(int fd, char *msg)
{
	(void)msg;
	printf(CYAN"\n[Exec LEFT]\n"RESET);
	printf(BLUE"Player [%d] -> [%s]\n"RESET, fd, "left");
	printf("players %d, pos-> y: %d x: %d d: %d\n", fd, g_players[fd].y, g_players[fd].x, g_players[fd].direction);

	if (--(g_players[fd].direction) < NORTH)
		g_players[fd].direction = WEST;
	g_players[fd].request_nb--;

	printf("players %d, pos-> y: %d x: %d d: %d\n", fd, g_players[fd].y, g_players[fd].x, g_players[fd].direction);
	if (!send_data(fd, RED"OK\n"RESET, strlen(RED"OK\n"RESET)))
		return (EXIT_FAILURE);
	// if (send_msg(fd, RED"OK\n"RESET, "Send [left]") == EXIT_FAILURE)
	// 	return (EXIT_FAILURE);
	//	maybe update graphic client regarding player position
	printf(CYAN"\n[LEFT success]\n"RESET);
	return (EXIT_SUCCESS);
}

/*
** big note::
** after acceptting the new client, we need to add it to global
** for players[newfd]; and malloc the t_player
** so when we can the player, we can call player[i]->direction
** for that certain player_fd.
*/
