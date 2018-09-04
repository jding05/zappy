/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_right.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sding <sding@student.42.us.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/17 18:25:25 by sding             #+#    #+#             */
/*   Updated: 2018/08/17 18:25:26 by sding            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** trun right 90 degrees
** 0:N , 1:E, 2:S, 3:W
*/
#include "../../inc/server.h"

int		cmd_right(t_players players, char *msg)
{
	(void)msg;
	printf(BLUE"Player [%d] -> [%s]"RESET, players.fd, "right");
	if (++(players.direction) > WEST)
		players.direction = NORTH;
	players.request_nb--;
	if (send_msg(players.fd, "OK", "Send [right]") == EXIT_FAILURE)
		return (EXIT_FAILURE);
	// maybe update graphic client regarding player position
	return (EXIT_SUCCESS);
}
