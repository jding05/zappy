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
#include "../../inc/server.h"

int		cmd_left(t_players players, char *msg)
{
	(void)msg;
	printf(BLUE"Player [%d] -> [%s]\n"RESET, players.fd, "left");
	if (--(players.direction) < NORTH)
		players.direction = WEST;
	players.request_nb--;
	if (send_msg(players.fd, "OK", "Send [left]") == EXIT_FAILURE)
		return (EXIT_FAILURE);
	//	maybe update graphic client regarding player position
	return (EXIT_SUCCESS);
}

/*
** big note::
** after acceptting the new client, we need to add it to global
** for players[newfd]; and malloc the t_player
** so when we can the player, we can call player[i]->direction
** for that certain player_fd.
*/
