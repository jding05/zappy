/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_connect_nbr.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sding <sding@student.42.us.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/17 18:27:00 by sding             #+#    #+#             */
/*   Updated: 2018/08/17 18:27:02 by sding            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** know the number of unused connections by the team
**
** since -c nb_clients_allowed, we calc nb_clients_allowed / nb_teams
**  -> to get the max_players in the team
*/

#include "../../include/server.h"

int		cmd_connect_nbr(int fd, char *msg)
{
	char	*str;

	(void)msg;
	printf(CYAN"\n[Exec CONNECT_NBR]\n"RESET);
	printf(BLUE"Player [%d] -> [%s]\n"RESET, fd, "connect_nbr");

	printf("Player %d, team: %s, nb_client %d\n", fd, g_teams[g_players[fd].team_id].team_name, g_teams[g_players[fd].team_id].nb_client);

	g_players[fd].request_nb--;
	bzero(g_env.buffer, MSG_SIZE);
	strcpy(g_env.buffer, RED);
	strcat(g_env.buffer, str = ft_itoa(g_teams[g_players[fd].team_id].nb_client));
	free(str);
	strcat(g_env.buffer, RESET);

	printf("Player %d, team: %s, nb_client %d\n", fd, g_teams[g_players[fd].team_id].team_name, g_teams[g_players[fd].team_id].nb_client);
	printf(CYAN"\n[CONNECT_NBR SUCCESS]\n"RESET);

	// if (send_msg(fd, connect_nbr, "Send [connect_nbr]") == EXIT_FAILURE)
	// 	return (EXIT_FAILURE);


	send_data(fd, g_env.buffer, MSG_SIZE);

	// if (send_msg(fd, connect_nbr, "Send [connect_nbr]") == EXIT_FAILURE)
	// 	return (EXIT_FAILURE);
	// update graphic client regarding player position
	return (EXIT_SUCCESS);
}
