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

int		cmd_put(int fd, char *msg)
{
	int res_i;

	printf(CYAN"\n[Exec PUT]\n"RESET);
	printf(BLUE"Player [%d] -> [%s %s]\n"RESET, fd, "put", msg);
	g_players[fd].request_nb--;
	if ((res_i = check_resource(msg)) == 7 || res_i == 0) // i think this can be handle in parse
	{

		// if (send_msg(fd, RED"KO\n"RESET, "Send [put]") == EXIT_FAILURE)
		// 	return (EXIT_FAILURE);

		send_data(fd, RED"PUT KO"RESET, MSG_SIZE);

	}
	if (g_players[fd].inventory[res_i] == 0)
	{
		// if (send_msg(fd, RED"KO\n"RESET, "Send [put]") == EXIT_FAILURE)
		// 	return (EXIT_FAILURE);

		send_data(fd, RED"PUT KO"RESET, MSG_SIZE);

	}
	if (g_players[fd].inventory[res_i] == 0)
	{
		send_data(fd, RED"KO\n"RESET, MSG_SIZE);

	}
	else
	{
		g_players[fd].inventory[res_i]--;
		g_env.map[g_players[fd].y][g_players[fd].x][res_i]++;
	}
	// printf("players %d, finish take -> %s\n", fd, msg);
	printf(CYAN"\n[PUT SUCCESS]\n"RESET);

	send_data(fd, RED"PUT OK"RESET, MSG_SIZE);

	// update graphic client regarding player position
	return (EXIT_SUCCESS);
}

// int		send_msg(int fd, char *status, char *error_msg)
// {
// 	if (send(fd, status, strlen(status), 0) == -1)
// 	{
// 		perror(error_msg);
// 		return (EXIT_FAILURE);
// 	}
// 	return (EXIT_SUCCESS);
// }
