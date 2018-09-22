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
	g_teams[id].egg[g_teams[id].egg_hatched].hatched = 1;
	g_teams[id].egg_hatched++;
	g_teams[id].nb_client++;
	printf(CYAN"\n[HATCH SUCCESS]\n"RESET);
}
