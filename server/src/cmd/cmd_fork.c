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

#include "../../include/server.h"

/*
** the unauthorized connection means the dead players
** once the egg 42/t hit, means it call this function
**
** 1. at the time we enqueue to the priority queue, we block this player,
**		after 42/t, when it executed, we unblock this player, so the player can
** 		execute other cmd it sent
** 2. we open one extra space for extra player that this team can accept
**		after the cmd_hatch() begin execute, but the we need to record the
**		position that the egg is laid, for the additonal player to start at
**		this place,
** -> then we call cmd_hatch put into queue (that marked 600/t after)
*/

static void    laid_egg(int fd)
{
	int	id;

	id = g_players[fd].team_id;
	g_teams[id].egg[g_teams[id].egg_laid].team_id = id;
	g_teams[id].egg[g_teams[id].egg_laid].egg_id = g_teams[id].egg_laid;
	g_teams[id].egg[g_teams[id].egg_laid].hatched = 0;
	g_teams[id].egg[g_teams[id].egg_laid].father_fd = fd;
	g_teams[id].egg[g_teams[id].egg_laid].y = g_players[fd].y;
	g_teams[id].egg[g_teams[id].egg_laid].x = g_players[fd].x;
	g_teams[id].egg_laid++;
}

void    cmd_fork(int fd, char *msg)
{
    printf(CYAN"\n[Exec FORK]\n"RESET);
    printf(BLUE"Player [%d] -> [%s]\n"RESET, fd, "fork");
	printf(BLUE"Player %d, block: %d\n"RESET, fd, g_players[fd].block);
    g_players[fd].request_nb--;
	(void)msg;
    laid_egg(fd);
    g_players[fd].block = 0;
	enqueue(fd, "hatch");

	printf(BLUE"Player %d, block: %d\n"RESET, fd, g_players[fd].block);
	printf(CYAN"\n[FORK SUCCESS]\n"RESET);

	send_data(fd, RED"FORK OK"RESET, MSG_SIZE);
}
