/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   accept_recv.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xzhu <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/22 21:43:41 by xzhu              #+#    #+#             */
/*   Updated: 2018/09/22 21:44:08 by xzhu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/server.h"

/*
** if a fd == listener, i.e. there is a new connection request, accept and
** connect
*/

static int	check_gfx(char *msg, int newfd)
{
	char		map_info[9];
	char		*rv;

	if (0 == strcmp(msg, "gfx") && g_env.gfx_fd <= 0)
	{
		g_env.gfx_fd = newfd;
		rv = ft_itoa(g_env.map_x);
		strcpy(map_info, rv);
		free(rv);
		strcat(map_info, ",");
		rv = ft_itoa(g_env.map_y);
		strcat(map_info, rv);
		free(rv);
		strcat(map_info, ",");
		rv = ft_itoa(g_env.nb_team);
		strcat(map_info, rv);
		free(rv);
		strcat(map_info, "@");
		send_data(newfd, map_info, 9);
		free(msg);
		return (2);
	}
	else if (0 == strcmp(msg, "gfx") && g_env.gfx_fd > 0)
		return (0 == close(newfd));
	return (0);
}

static void	accept_norm(int newfd, fd_set *master, int *fdmax,
						struct sockaddr_storage remoteaddr)
{
	char	*msg;
	char	remote_ip[INET6_ADDRSTRLEN];

	msg = get_n_x_y(newfd);
	send_data(newfd, msg, MSG_SIZE);
	free(msg);
	FD_SET(newfd, master);
	if (newfd > *fdmax)
		*fdmax = newfd;
	printf("new connection from %s on socket %d\n",
			inet_ntop(remoteaddr.ss_family,
				get_in_addr((struct sockaddr*)&remoteaddr),
				remote_ip, INET6_ADDRSTRLEN), newfd);
}

void		s_select_accept(int fd, fd_set *master, int *fdmax)
{
	int						newfd;
	struct sockaddr_storage remoteaddr;
	socklen_t				addrlen;
	char					*msg;
	int						check_return;

	addrlen = sizeof(remoteaddr);
	if ((newfd = accept(fd, (struct sockaddr *)&remoteaddr, &addrlen)) == -1)
		perror("accept");
	msg = recv_data(newfd, MSG_SIZE);
	if ((check_return = check_gfx(msg, newfd)) > 0)
	{
		if (check_return == 2)
		{
			FD_SET(newfd, master);
			if (newfd > *fdmax)
				*fdmax = newfd;
		}
		return ;
	}
	send_data(newfd, WELCOME, MSG_SIZE);
	s_add_to_team(msg, newfd) == EXIT_FAILURE ?
		close(newfd) : accept_norm(newfd, master, fdmax, remoteaddr);
	free(msg);
}

/*
** if a fd != listener, i.e. it's already connected to the server,
** receive the buffer then store the data into structs.
*/

void		s_select_recv(int fd, fd_set *master)
{
	char	*req;

	if (!(req = recv_data(fd, MSG_SIZE)))
	{
		close(fd);
		if (fd == g_env.gfx_fd)
			g_env.gfx_fd = 0;
		s_clear_player(fd);
		FD_CLR(fd, master);
		return (free(req));
	}
	if (g_players[fd].dead == 1)
	{
		send_data(fd, RED"Dead player cannot make request"RESET, MSG_SIZE);
		return (free(req));
	}
	if (g_players[fd].request_nb < 10)
	{
		send_data(fd, "received", MSG_SIZE);
		if (enqueue(fd, req) == 1)
			g_players[fd].request_nb++;
	}
	else
		send_data(fd, "request_nb limit", MSG_SIZE);
	free(req);
}
