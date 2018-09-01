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

#include <server.h>

int		cmd_connect_nbr(t_players players, char *msg)
{
    char    *connect_nbr;
    t_team  *team;

    printf(BLUE"Player [$d] -> [%s]\n"RESET, players.fd, "connect_nbr");
    players.request_nb--;
    team = g_env->team;
    while (team)
        (team->team_number != players.team_id) ? team = team->next : break;
    connect_nbr = ft_itoa(team->max_players - team->connected_players);
    if (send(players.fd, connect_nbr, strlen(connect_nbr), 0) == -1)
		perror("Send [connect_nbr]");
    // update graphic client regarding player position
    return (EXIT_SUCCESS);
}