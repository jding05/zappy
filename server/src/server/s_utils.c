/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   s_utils.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfeng <zfeng@student.42.us.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/07 13:41:07 by zfeng             #+#    #+#             */
/*   Updated: 2018/09/07 13:41:09 by zfeng            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"
#include "parse.h"

t_player	g_players[MAX_FD];
t_team		g_teams[MAX_TEAM];
t_env		g_env;
t_cmdq		*g_cmdq;

/*
** print out the error message then return failure
*/

int		perror_rv(char *errmsg)
{
	write(2, errmsg, strlen(errmsg));
	return (EXIT_FAILURE);
}

/*
** initialize a player's data
** or
** reset a player's data when the player_client is terminated
*/

void	s_init_player(int fd)
{
	g_players[fd].fd = fd;
	g_players[fd].nb_req = 0;
	memset(g_players[fd].inventory, 0, 7);
	memset(g_players[fd].pos, -1, 2);
	g_players[fd].level = 0;
}

/*
** when a new player is connected, add it to a team
*/

int		s_add_to_team(char *team_name, int fd)
{
	int		i;

	i = 0;
	while (*g_teams[i].team_name)
	{
		if (strcmp(g_teams[i].team_name, team_name) == 0)
		{
			if (g_teams[i].nb_client == 0)
			{
				send_msg(fd, TEAM_FULL);
				return (perror_rv("team is full\n")); 
			}
			s_init_player(fd);
			g_players[fd].team_id = i;
			g_teams[i].nb_client--;
			send_msg(fd, TEAM_JOINED);
			return (EXIT_SUCCESS);
		}
		i++;
	}
	send_msg(fd, TEAM_NOT_FOUND);
	return (perror_rv("team not found\n"));
}

void	s_exec_cmd(t_cmdq **head)
{
	int		i;

	if (!*head)
		return ;
	i = 0;
	while (i < 13)
	{
		if (strcmp((*head)->cmd, g_cmds[i].cmd) == 0)
		{
			g_cmds[i].func();
			g_players[(*head)->fd].nb_req--;
			//send((*head)->fd, "OK\n", 3, 0);
			//send_msg((*head)->fd, "OK\n");
			dequeue(head);
			return ;
		}
		i++;
	}
}