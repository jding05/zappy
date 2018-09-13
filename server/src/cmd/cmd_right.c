/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_right.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sding <sding@student.42.us.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/17 18:25:25 by sding             #+#    #+#             */
/*   Updated: 2018/08/17 18:25:26 by sding            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** trun right 90 degrees
** 0:N , 1:E, 2:S, 3:W
*/
#include "../../include/server.h"

int		cmd_right(int fd, char *msg)
{
	(void)msg;
	printf(CYAN"\n[Exec RIGHT]\n"RESET);
	printf(BLUE"Player [%d] -> [%s]"RESET, fd, "right");
	printf("players %d, pos-> y: %d x: %d d: %d\n", fd, g_players[fd].y, g_players[fd].x, g_players[fd].direction);
	if (++(g_players[fd].direction) > WEST)
		g_players[fd].direction = NORTH;
	g_players[fd].request_nb--;

	printf("players %d, pos-> y: %d x: %d d: %d\n", fd, g_players[fd].y, g_players[fd].x, g_players[fd].direction);
	printf(CYAN"\n[RIGHT SUCCESS]\n"RESET);

	// if (send_msg(fd, RED"OK\n"RESET, "Send [right]") == EXIT_FAILURE)
	// 	return (EXIT_FAILURE);


	send_data(fd, RED"RIGHT OK"RESET, MSG_SIZE);

	// maybe update graphic client regarding player position
	return (EXIT_SUCCESS);
}
