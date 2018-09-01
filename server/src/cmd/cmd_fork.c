/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_fork.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sding <sding@student.42.us.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/17 18:26:52 by sding             #+#    #+#             */
/*   Updated: 2018/08/17 18:26:53 by sding            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <server.h>

/*
** the unauthorized connection means the dead players
** once the egg 42/t hit, means it call this function
** -> then we call cmd_hatch put into queue (that marked 600/t after)
*/
int     cmd_fork(t_players players, char *msg)
{
    printf(BLUE"Player [$d] -> [%s]\n"RESET, players.fd, "fork");
    players.request_nb--;

    if (send(players.fd, "OK", 2, 0) == -1)
	    perror("Send [fork]");
    return(EXIT_SUCCESS);
}
