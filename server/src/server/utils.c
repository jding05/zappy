/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfeng <zfeng@student.42.us.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/07 13:41:07 by zfeng             #+#    #+#             */
/*   Updated: 2018/09/10 23:07:43 by zfeng            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"
#include "parse.h"

t_player	g_players[MAX_FD];
t_team		g_teams[MAX_TEAM];
t_env		g_env;
t_reqq		*g_reqq;

/*
** print out the error message then return failure
*/

int		perror_rv(char *errmsg)
{
	write(2, errmsg, strlen(errmsg));
	return (EXIT_FAILURE);
}

char	*ft_strnew(size_t size)
{
	char	*new;

	new = (char*)malloc(sizeof(char) * (size + 1));
	memset(new, 0, size + 1);
	return (new);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*new;

	if (s1 && s2)
	{
		new = ft_strnew(strlen(s1) + strlen(s2));
		if (!new)
			return (NULL);
		strcpy(new, s1);
		strcat(new, s2);
		return (new);
	}
	return (NULL);
}

/*
** initialize a player's data
*/

void	init_player(int fd)
{
	static int	player_id = 0;

	g_players[fd].player_id = player_id++;
	g_players[fd].fd = fd;
	g_players[fd].nb_req = 0;
	memset(g_players[fd].inventory, 0, 7);
	memset(g_players[fd].pos, -1, 2);
	g_players[fd].level = 0;
	printf("player_id = %d\n", g_players[fd].player_id);
}

/*
** reset a player's data when the player_client is terminated
*/

void	reset_player(int fd)
{
	g_players[fd].nb_req = 0;
	memset(g_players[fd].inventory, 0, 7);
	memset(g_players[fd].pos, -1, 2);
	g_players[fd].level = 0;
}

/*
** when a new player is connected, add it to a team
*/

int		add_to_team(char *team_name, int fd)
{
	int		i;
	char	*msg;

	i = 0;
	while (*g_teams[i].team_name)
	{
		if (strcmp(g_teams[i].team_name, team_name) == 0)
		{
			if (g_teams[i].nb_client == 0)
			{
				send_data(fd, TEAM_FULL, MSG_SIZE);
				return (EXIT_FAILURE); 
			}
			init_player(fd);
			g_players[fd].team_id = i;
			g_teams[i].nb_client--;
			g_teams[i].connect_nbr++;
			msg = ft_strjoin("joined team ", team_name);
			send_data(fd, msg, MSG_SIZE);
			free(msg);
			return (EXIT_SUCCESS);
		}
		i++;
	}
	send_data(fd, NAME_NOT_FOUND, MSG_SIZE);
	return (EXIT_FAILURE);
}

void	exec_cmd(t_reqq **head)
{
	int		i;

	if (!*head)
		return ;
	i = 0;
	printf("cmd = %s | param = %s\n", (*head)->cmd, (*head)->param);
	while (i < 14)
	{
		if (strcmp((*head)->cmd, g_cmd_table[i].cmd) == 0)
		{
			g_cmd_table[i].func(*head);
			g_players[(*head)->fd].nb_req--;
			dequeue(head);
			return ;
		}
		i++;
	}
}
