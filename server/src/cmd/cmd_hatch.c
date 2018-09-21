/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_hatch.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sding <sding@student.42.us.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/31 15:01:35 by sding             #+#    #+#             */
/*   Updated: 2018/08/31 15:01:36 by sding            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/server.h"

/*
** [ hatch ] is not any command sent from client (player)
** 		after 600/t waiting for the egg to hatch,
*/

void	cmd_hatch(int fd, char *msg)
{
	int	id;

	id = g_players[fd].team_id;
	(void)msg;
	printf(CYAN"\n[Exec HATCH]\n"RESET);
	printf(BLUE"Player [%d] -> [%s]\n"RESET, fd, "hatch");
	printf("team: %s, nb_client: %d, egg_hatch: %d\n", g_teams[id].team_name,
		g_teams[id].nb_client, g_teams[id].egg_hatched);
	if (g_teams[id].egg[g_teams[id].egg_hatched].ffd == fd)
		g_teams[id].egg[g_teams[id].egg_hatched].hatched = 1;
	else
		printf("[hatch wrong egg, egg.ffd: %d, cmd_hatch fd: %d]\n",
		g_teams[id].egg[g_teams[id].egg_hatched].ffd, fd);
	g_teams[id].egg_hatched++;
	g_teams[id].nb_client++;

	printf("team: %s, nb_client: %d, egg_hatch: %d\n", g_teams[id].team_name,
		g_teams[id].nb_client, g_teams[id].egg_hatched);
	printf(CYAN"\n[HATCH SUCCESS]\n"RESET);
}
