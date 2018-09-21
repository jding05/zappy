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

#include "../../include/server.h"

void	cmd_put(int fd, char *msg)
{
	int res_i;

	printf(CYAN"\n[Exec PUT]\n"RESET);
	printf(BLUE"Player [%d] -> [%s %s]\n"RESET, fd, "put", msg);
	g_players[fd].request_nb--;
	if ((res_i = check_resource(msg)) == 7 || res_i == 0 ||
			g_players[fd].inventory[res_i] == 0)
	{
		send_data(fd, RED"PUT KO"RESET, MSG_SIZE);
		return ;
	}
	else
	{
		g_players[fd].inventory[res_i]--;
		g_env.map[g_players[fd].y][g_players[fd].x][res_i]++;
	}
	printf(CYAN"\n[PUT SUCCESS]\n"RESET);
	send_data(fd, RED"PUT OK"RESET, MSG_SIZE);
}
