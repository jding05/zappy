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

#include "../../inc/server.h"

int		cmd_hatch(t_players players, char *msg)
{
	printf(BLUE"Player [%d] -> [%s]\n"RESET, players.fd, "hatch");
	players.block = 0;
	g_teams[players.team_id].egg_hatched++;
	return (EXIT_SUCCESS);
}
