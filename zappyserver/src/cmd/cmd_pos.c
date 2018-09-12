/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_pos.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sding <sding@student.42.us.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/05 21:04:03 by sding             #+#    #+#             */
/*   Updated: 2018/09/05 21:04:04 by sding            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/server.h"

int		cmd_pos(int fd, char *msg)
{
	char	*str;

	(void)msg;
	printf(CYAN"\n[Exec POS]\n"RESET);
	printf(BLUE"Player [%d] -> [%s]\n"RESET, fd, "pos");
	bzero(g_env.buffer, 4096);
	strcpy(g_env.buffer, RED);
	strcat(g_env.buffer, "[player ");
	strcat(g_env.buffer, (str = ft_itoa(fd)));
	free(str);
	strcat(g_env.buffer, " ] y: ");
	strcat(g_env.buffer, (str = ft_itoa(g_players[fd].y)));
	free(str);
	strcat(g_env.buffer, " x: ");
	strcat(g_env.buffer, (str = ft_itoa(g_players[fd].x)));
	free(str);
	strcat(g_env.buffer, " direction: ");
	if (g_players[fd].direction == NORTH)
		strcat(g_env.buffer, "north");
	else if (g_players[fd].direction == EAST)
		strcat(g_env.buffer, "east");
	else if (g_players[fd].direction == SOUTH)
		strcat(g_env.buffer, "south");
	else if (g_players[fd].direction == WEST)
		strcat(g_env.buffer, "west");
	// strcat(g_env.buffer, (str = ft_itoa(g_players[fd].direction)));
	// free(str);
	strcat(g_env.buffer, "\n");
	strcat(g_env.buffer, RESET);
	if (send_msg(fd, g_env.buffer, "Send [POS]") == EXIT_FAILURE)
		return (EXIT_FAILURE);
	g_players[fd].request_nb--;

	//	maybe update graphic client regarding player position
	printf(CYAN"\n[POS success]\n"RESET);
	return (EXIT_SUCCESS);
}
