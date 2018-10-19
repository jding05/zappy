/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   s_utils.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfeng <zfeng@student.42.us.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/07 13:41:07 by zfeng             #+#    #+#             */
/*   Updated: 2018/09/19 23:35:23 by xzhu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/server.h"

/*
** get sockaddr, IPv4 or IPv6
*/

void			*get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET)
		return (&(((struct sockaddr_in*)sa)->sin_addr));
	return (&(((struct sockaddr_in6*)sa)->sin6_addr));
}

/*
** iterate through the return link list of sockets from getaddrinfo()
*/

int				s_iter_sock(struct addrinfo *ai,
							struct protoent *proto, int reuse)
{
	struct addrinfo *p;
	int				listener;

	p = ai;
	while (p)
	{
		listener = socket(p->ai_family, p->ai_socktype, proto->p_proto);
		if (listener < 0)
		{
			p = p->ai_next;
			continue;
		}
		setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int));
		if (bind(listener, p->ai_addr, p->ai_addrlen) < 0)
		{
			close(listener);
			continue;
		}
		break ;
	}
	if (p == NULL)
		return (-1);
	freeaddrinfo(ai);
	return (listener);
}

char			*get_n_x_y(int fd)
{
	char	*rv;
	char	*msg;

	if (NULL == (msg = (char*)malloc(sizeof(char) * 10)))
		return (NULL);
	memset(msg, 0, 10);
	rv = ft_itoa(g_teams[g_players[fd].team_id].nb_client);
	strcpy(msg, rv);
	free(rv);
	strcat(msg, "\n");
	rv = ft_itoa(g_env.map_x);
	strcat(msg, rv);
	free(rv);
	strcat(msg, " ");
	rv = ft_itoa(g_env.map_y);
	strcat(msg, rv);
	free(rv);
	return (msg);
}

/*
** when a new player is connected, add it to a team
*/

int				s_add_to_team(char *team_name, int fd)
{
	ADD_TEAM_VARS;
	i = -1;
	while (*g_teams[++i].team_name)
	{
		if (strcmp(g_teams[i].team_name, team_name) == 0)
		{
			if (!g_teams[i].nb_client && !send_data(fd, TEAM_FULL, MSG_SIZE))
				return (EXIT_FAILURE);
			(egg_id = g_teams[i].max_players - g_teams[i].cplayers++) > 0 ?
				s_init_new_player(fd) : s_init_egg_player(fd, i, abs(egg_id));
			g_env.active++;
			g_players[fd].team_id = i;
			g_teams[i].nb_client--;
			msg = ft_strjoin("joined team ", team_name);
			send_data(fd, msg, MSG_SIZE);
			free(msg);
			msg = get_gfx_data();
			if (g_env.gfx_fd > 0)
				send_data(g_env.gfx_fd, msg, MSG_SIZE);
			free(msg);
			return (EXIT_SUCCESS);
		}
	}
	send_data(fd, TEAM_NOT_FOUND, MSG_SIZE);
	return (EXIT_FAILURE);
}

/*
** At first, we make the game with the [ event-driven ],
**	-> only run if there is an event
**  , but if no player send the command to server
**		-> then the server WON'T update,
**			and won't check if the player is dead or not
**
** Therefore, we change our game to the [ time-driven ],
** -> instead of setting the timeout by (1.) the first queue event timeout time
**							or  (2.) empty queue -> set as NULL, blocking status
** **-> we change the timeout to the game_tick time for above two cases,
** -> then we can check the player's status (dead or not) every time
** 	[now become the Real-Time-Driven Sever]
*/

struct timeval	*set_timeout_alarm(void)
{
	struct timeval	*timeout;

	timeout = (struct timeval *)malloc(sizeof(struct timeval));
	timeout->tv_sec = g_env.ms_pre_tick == 1000000 ? 1 : 0;
	timeout->tv_usec = g_env.ms_pre_tick == 1000000 ? 0 : g_env.ms_pre_tick;
	return (timeout);
}
