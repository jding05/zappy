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
	strcat(msg, "\n");
	rv = ft_itoa(g_env.map_x);
	strcat(msg, rv);
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
			if (g_teams[i].nb_client == 0)
			{
				send_data(fd, TEAM_FULL, MSG_SIZE);
				return (EXIT_FAILURE);
			}
			if ((egg_id = g_teams[i].max_players - g_teams[i].cplayers++) > 0)
				s_init_new_player(fd);
			else
				s_init_egg_player(fd, i, abs(egg_id));
			g_players[fd].team_id = i;
			g_teams[i].nb_client--;
			msg = ft_strjoin("joined team ", team_name);
			send_data(fd, msg, MSG_SIZE);
			free(msg);
			return (EXIT_SUCCESS);
		}
	}
	send_data(fd, TEAM_NOT_FOUND, MSG_SIZE);
	return (EXIT_FAILURE);
}

struct timeval	*set_timeout_alarm(void)
{
	long int		time_diff;
	struct timeval	*timeout;
	struct timeval	now;

	timeout = NULL;
	if (!g_env.queue_head)
		return (NULL);
	gettimeofday(&now, NULL);
	time_diff = (g_env.queue_head->exec_time.tv_sec - now.tv_sec) * 1000000
		+ (g_env.queue_head->exec_time.tv_usec - now.tv_usec);
	timeout = (struct timeval *)malloc(sizeof(struct timeval));
	if (time_diff <= 0)
	{
		timeout->tv_sec = 0;
		timeout->tv_usec = 0;
	}
	else
	{
		timeout->tv_sec = time_diff / 1000000;
		timeout->tv_usec = time_diff % 1000000;
	}
	return (timeout);
}
