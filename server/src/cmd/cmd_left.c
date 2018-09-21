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
#include "../../include/server.h"

void	cmd_left(int fd, char *msg)
{
	(void)msg;
	printf(CYAN"\n[Exec LEFT]\n"RESET);
	printf(BLUE"Player [%d] -> [%s]\n"RESET, fd, "left");
	if (--(g_players[fd].direction) < NORTH)
		g_players[fd].direction = WEST;
	g_players[fd].request_nb--;
	send_data(fd, RED"LEFT OK"RESET, MSG_SIZE);
	printf(CYAN"\n[LEFT success]\n"RESET);
}
