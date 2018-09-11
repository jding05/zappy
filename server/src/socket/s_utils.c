/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   s_utils.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfeng <zfeng@student.42.us.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/07 13:41:07 by zfeng             #+#    #+#             */
/*   Updated: 2018/09/08 17:17:39 by sding            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../inc/server.h"

int		perror_rv(char *errmsg)
{
	write(2, errmsg, strlen(errmsg));
	return (EXIT_FAILURE);
}

void	init_live(int fd)
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

void	s_init_egg_player(int fd, int team_id, int egg_id)
{
	g_players[fd].fd = fd;
	g_players[fd].request_nb = 0;
	memset(g_players[fd].inventory, 0, 7);
	g_players[fd].y = g_teams[team_id].egg[egg_id].y;
	g_players[fd].x = g_teams[team_id].egg[egg_id].x;
	g_players[fd].request_nb = 0;
	g_players[fd].level = 1;
	g_players[fd].alive = 1;
	g_players[fd].dead = 0;
	g_players[fd].block = 0;
	g_players[fd].direction = rand() % 4;
	init_live(fd);
	// printf("[use the egg player %d, y: %d, x: %d]\n", fd, g_players[fd].y, g_players[fd].x);
}
/*
** reset a player's data when the player_client is terminated
*/

void	s_init_new_player(int fd)
{
	g_players[fd].fd = fd;
	g_players[fd].request_nb = 0;
	memset(g_players[fd].inventory, 0, 7);
	g_players[fd].inventory[1] = 10;
	g_players[fd].inventory[2] = 10;
	g_players[fd].inventory[3] = 10;
	g_players[fd].inventory[4] = 10;
	g_players[fd].inventory[5] = 10;
	g_players[fd].inventory[6] = 10;
	g_players[fd].y = 2;
	g_players[fd].x = 5;
	// g_players[fd].y = rand() % g_env.map_y;
	// g_players[fd].x = rand() % g_env.map_x;
	g_players[fd].request_nb = 0;
	g_players[fd].level = 1;
	g_players[fd].alive = 1;
	g_players[fd].dead = 0;
	g_players[fd].block = 0;
	g_players[fd].direction = 0;
	// g_players[fd].direction = rand() % 4;
	init_live(fd);
}

/*
** when a new player is connected, add it to a team
*/

int		s_add_to_team(char *team_name, int fd)
{
	int		i;
	int		egg_id;

 	i = 0;
	while (*g_teams[i].team_name)
	{
		if (strcmp(g_teams[i].team_name, team_name) == 0)
		{
			if (g_teams[i].nb_client == 0)
			{
				// s_send_msg(fd, "#team is full\nBYE 😕\n");
				s_send_msg(fd, TEAM_FULL);
				return (perror_rv("team is full\n"));
			}
			if ((egg_id = g_teams[i].max_players - g_teams[i].connected_players++) > 0)
				s_init_new_player(fd);
			else
				s_init_egg_player(fd, i, egg_id);
			g_players[fd].team_id = i;
			g_teams[i].nb_client--;
			// s_send_msg(fd, "joind team\n");
			s_send_msg(fd, TEAM_JOINED);
			return (EXIT_SUCCESS);
		}
		i++;
	}
	// s_send_msg(fd, "#team not found\nBYE 😕\n");
	s_send_msg(fd, TEAM_NOT_FOUND);
	return (perror_rv("team not found\n"));
}

// didn't write s_exec_cmd()
// void	s_exec_cmd(t_cmdq **head)
// {
// 	int		i;
//
// 	if (!*head)
// 		return ;
// 	i = 0;
// 	while (i < 13)
// 	{
// 		if (strcmp((*head)->cmd, g_cmds[i].cmd) == 0)
// 		{
// 			g_cmds[i].func();
// 			g_players[(*head)->fd].nb_req--;
// 			//send((*head)->fd, "OK\n", 3, 0);
// 			//send_msg((*head)->fd, "OK\n");
// 			dequeue(head);
// 			return ;
// 		}
// 		i++;
// 	}
// }
