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

#include "../../inc/server.h"

int		cmd_connect_nbr(t_players players, char *msg)
{
	char	*connect_nb;
	int		team_id;

	printf(BLUE"Player [%d] -> [%s]\n"RESET, players.fd, "connect_nbr");
	players.request_nb--;
	team_id = players.team_id;
	connect_nb = ft_itoa(g_teams[team_id].max_players - \
		g_teams[team_id].connected_players + g_teams[team_id].egg_hatched);// need to double check for egg_used
	if (send_msg(players.fd, connect_nb, "Send [connect_nbr]") == EXIT_FAILURE)
		return (EXIT_FAILURE);
	// update graphic client regarding player position
	return (EXIT_SUCCESS);
}
