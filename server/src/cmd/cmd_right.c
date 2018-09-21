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

void	cmd_right(int fd, char *msg)
{
	(void)msg;
	printf(CYAN"\n[Exec RIGHT]\n"RESET);
	printf(BLUE"Player [%d] -> [%s]"RESET, fd, "right");
	if (++(g_players[fd].direction) > WEST)
		g_players[fd].direction = NORTH;
	g_players[fd].request_nb--;
	printf(CYAN"\n[RIGHT SUCCESS]\n"RESET);
	send_data(fd, RED"RIGHT OK"RESET, MSG_SIZE);
}
