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

#include "../../inc/server.h"

/*
** the unauthorized connection means the dead players
** once the egg 42/t hit, means it call this function
** -> then we call cmd_hatch put into queue (that marked 600/t after)
*/
int     cmd_fork(int fd, char *msg)
{
    printf(CYAN"\n[Exec FORK]\n"RESET);
    printf(BLUE"Player [%d] -> [%s]\n"RESET, fd, "fork");
	printf(BLUE"Player %d, block: %d\n"RESET, fd, g_players[fd].block);
    g_players[fd].request_nb--;
	(void)msg;
    laid_egg(fd);
    g_players[fd].block = 0;
	push_cmd_hatch(fd);

	printf(BLUE"Player %d, block: %d\n"RESET, fd, g_players[fd].block);
	printf(CYAN"\n[RIGHT SUCCESS]\n"RESET);
	send_data(fd, RED"OK\n"RESET, strlen(RED"OK\n"RESET));
	// if (send_msg(fd, RED"OK\n"RESET, "Send [fork]") == EXIT_FAILURE)
	// 	return (EXIT_FAILURE);
    return(EXIT_SUCCESS);
}

void	update_live(int fd, int nb_food)
{
	printf(BLUE"Player [%d] -> [Update %d live]\n"RESET, fd, nb_food);
	g_players[fd].live.tv_sec += (g_players[fd].live.tv_usec + nb_food * 126 * g_env.ms_pre_tick) / 1000000;
	g_players[fd].live.tv_usec = (g_players[fd].live.tv_usec + nb_food * 126 * g_env.ms_pre_tick) % 1000000;
}

void    push_cmd_hatch(int fd)
{
	enqueue(fd, "hatch");
}

void    laid_egg(int fd)
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
