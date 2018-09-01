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
#include <server.h>

int		cmd_right(t_players players, char *msg)
{
	printf(BLUE"Player [$d] -> [%s]"RESET, players.fd, "right");
	if (++(players.direction) > WEST)
		players.direction = NORTH;
	players.request_nb--;
	if (send(players.fd, "OK", 2, 0) == -1)
		perror("Send [right]");
	// maybe update graphic client regarding player position
	return (EXIT_SUCCESS);
}
