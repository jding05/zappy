/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_player.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sding <sding@student.42.us.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/20 21:53:50 by sding             #+#    #+#             */
/*   Updated: 2018/09/20 21:53:51 by sding            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/server.h"

static void	init_live(int fd)
{
	struct timeval	curr_time;

	gettimeofday(&curr_time, NULL);
	gettimeofday(&(g_players[fd].live), NULL);
	g_players[fd].live.tv_sec = curr_time.tv_sec;
	g_players[fd].live.tv_usec = curr_time.tv_usec;
	g_players[fd].block_time.tv_sec = curr_time.tv_sec;
	g_players[fd].block_time.tv_usec = curr_time.tv_usec;
	update_live(fd, 1000);
}

void		s_init_egg_player(int fd, int team_id, int egg_id)
{
	g_players[fd].fd = fd;
	g_players[fd].player_id = g_player_id++;
	g_players[fd].request_nb = 0;
	memset(g_players[fd].inventory, 0, sizeof(int) * 7);
	g_players[fd].y = g_teams[team_id].egg[egg_id].y;
	g_players[fd].x = g_teams[team_id].egg[egg_id].x;
	g_players[fd].request_nb = 0;
	g_players[fd].level = 1;
	g_players[fd].alive = 1;
	g_players[fd].dead = 0;
	g_players[fd].block = 0;
	g_players[fd].direction = rand() % 4;
	init_live(fd);
}

/*
** reset a player's data when the player_client is terminated
*/

void		s_init_new_player(int fd)
{
	g_players[fd].fd = fd;
	g_players[fd].player_id = g_player_id++;
	g_players[fd].request_nb = 0;
	memset(g_players[fd].inventory, 0, sizeof(int) * 7);
	g_players[fd].inventory[1] = 9;
	g_players[fd].inventory[2] = 8;
	g_players[fd].inventory[3] = 10;
	g_players[fd].inventory[4] = 5;
	g_players[fd].inventory[5] = 6;
	g_players[fd].inventory[6] = 1;
	g_players[fd].y = rand() % g_env.map_y;
	g_players[fd].x = rand() % g_env.map_x;
	g_players[fd].level = 1;
	g_players[fd].left = 0;
	g_players[fd].status = 0;
	g_players[fd].alive = 1;
	g_players[fd].dead = 0;
	g_players[fd].block = 0;
	g_players[fd].direction = rand() % 4;
	init_live(fd);
}

static void	s_clear_unblock(int fd)
{
	int i;

	i = -1;
	if (g_players[fd].block == 1)
	{
		if (g_players[fd].status == 1)
		{
			while (++i < MAX_FD)
			{
				if (g_players[fd].block_time.tv_sec ==
					g_players[i].block_time.tv_sec &&
					g_players[fd].block_time.tv_usec ==
					g_players[i].block_time.tv_usec  && i != fd)
				{
					g_players[i].block = 0;
					g_players[i].status = 0;
					send_data(i, RED"INCANTATION KO"RESET, MSG_SIZE);
				}
			}
		}
		else if (g_players[fd].status == 2)
			g_teams[g_players[fd].team_id].egg_enqueued--;
	}
}

void		s_clear_player(int fd)
{
	if (g_players[fd].dead != 1)
	{
		g_teams[g_players[fd].team_id].cplayers--;
		g_teams[g_players[fd].team_id].nb_client++;
	}
	s_clear_unblock(fd);
	g_players[fd].fd = 0;
	g_players[fd].player_id = 0;
	g_players[fd].request_nb = 0;
	memset(g_players[fd].inventory, 0, sizeof(int) * 7);
	g_players[fd].y = 0;
	g_players[fd].x = 0;
	g_players[fd].level = 0;
	g_players[fd].left = 0;
	g_players[fd].status = 0;
	g_players[fd].alive = 0;
	g_players[fd].dead = 0;
	g_players[fd].block = 0;
	g_players[fd].direction = 0;
	memset(&g_players[fd].live, 0, sizeof(struct timeval));
	memset(&g_players[fd].block_time, 0, sizeof(struct timeval));
}
