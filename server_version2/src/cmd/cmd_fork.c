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

static void	hatching_process(int team_id)
{
	struct timeval	exec_time;
	t_hatch			*hatch_egg;
	t_hatch			*tmp;

	gettimeofday(&exec_time, NULL);
	exec_time.tv_sec += (exec_time.tv_usec + 600 *
						g_env.ms_pre_tick) / 1000000;
	exec_time.tv_usec = (exec_time.tv_usec + 600 *
						g_env.ms_pre_tick) % 1000000;
	hatch_egg = (t_hatch*)malloc(sizeof(t_hatch));
	hatch_egg->exec_time = exec_time;
	hatch_egg->team_id = team_id;
	hatch_egg->next = NULL;
	if (g_env.hatching == NULL)
		g_env.hatching = hatch_egg;
	else
	{
		tmp = g_env.hatching;
		while (tmp->next != NULL)
			tmp = tmp->next;
		tmp->next = hatch_egg;
	}
}

static void	laid_egg(int fd)
{
	int				id;

	id = g_players[fd].team_id;
	g_teams[id].egg[g_teams[id].egg_laid].team_id = id;
	g_teams[id].egg[g_teams[id].egg_laid].egg_id = g_teams[id].egg_laid;
	g_teams[id].egg[g_teams[id].egg_laid].hatched = 0;
	g_teams[id].egg[g_teams[id].egg_laid].ffd = fd;
	g_teams[id].egg[g_teams[id].egg_laid].y = g_players[fd].y;
	g_teams[id].egg[g_teams[id].egg_laid].x = g_players[fd].x;
	g_teams[id].egg_laid++;
	g_env.eggs++;
	hatching_process(id);
}

void		cmd_fork(int fd, char *msg, int player_id)
{
	printf(CYAN"\n[Exec FORK]\n"RESET);
	printf(BLUE"Player [%d] -> [%s]\n"RESET, fd, "fork");
	if (player_id != g_players[fd].player_id)
		return (error_return("player_id does not match"));
	g_players[fd].request_nb--;
	(void)msg;
	laid_egg(fd);
	printf(CYAN"\n[FORK SUCCESS]\n"RESET);
	send_data(fd, RED"FORK OK"RESET, MSG_SIZE);
	g_players[fd].block = 0;
	g_players[fd].status = 0;
}
