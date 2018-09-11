/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfeng <zfeng@student.42.us.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/31 14:54:00 by zfeng             #+#    #+#             */
/*   Updated: 2018/09/10 23:07:37 by zfeng            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"
#include "parse.h"

t_player	g_players[MAX_FD];
t_team		g_teams[MAX_TEAM];
t_env		g_env;
t_reqq		*g_reqq;

/*
** get sockaddr, IPv4 or IPv6
*/

void	*get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET)
		return (&(((struct sockaddr_in*)sa)->sin_addr));
	return (&(((struct sockaddr_in6*)sa)->sin6_addr));
}

/*
** iterate through the return link list of sockets from getaddrinfo()
*/

int		iter_sock(struct addrinfo *ai, struct protoent *proto, int reuse)
{
	struct addrinfo *p;
	int				listener;

	p = ai;
	while (p)
	{
		//listener = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
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

/*
** create a listener socket
*/

int		create_socket(char* port, int reuse)
{
	int		listener;
	int		rv;

	SOCKET_VARS;
	proto = getprotobyname("tcp");
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	if ((rv = getaddrinfo(NULL, port, &hints, &ai)) != 0)
		perror_rv((char*)gai_strerror(rv));
	listener = iter_sock(ai, proto, reuse);
	return (listener);
}

/*
** if a fd == listener, i.e. there is a new connection request, accept and
** connect
*/

void	select_accept(int fd, fd_set *master, int *fdmax)
{
	int						newfd;
	struct sockaddr_storage remoteaddr;
	socklen_t				addrlen;
	char					remote_ip[INET6_ADDRSTRLEN];
	char					*team_name;

	addrlen = sizeof(remoteaddr);
	if ((newfd = accept(fd, (struct sockaddr *)&remoteaddr, &addrlen)) == -1)
		perror("accept");
	send_data(newfd, WELCOME, MSG_SIZE);
	team_name = recv_data(newfd, MAX_TEAM_NAME);
	if (add_to_team(team_name, newfd) == EXIT_FAILURE)
		close(newfd);	
	else
	{
		FD_SET(newfd, master);
		if (newfd > *fdmax)
			*fdmax = newfd;
		printf("new connection from %s on socket %d\n",
				inet_ntop(remoteaddr.ss_family, 
					get_in_addr((struct sockaddr*)&remoteaddr), 
					remote_ip, INET6_ADDRSTRLEN), newfd);	
	}
}

/*
** if a fd != listener, i.e. it's already connected to the server,
** receive the buffer then store the data into structs.
*/

void	select_recv(int fd, fd_set *master)
{
	char	*req;

	req = recv_data(fd, REQ_SIZE);
	if (!req)
		return ;
	if (g_players[fd].nb_req < 11)
	{
		// printf("nb_req = %d\n", g_players[fd].nb_req);
		enqueue(&g_reqq, fd, req);
		g_players[fd].nb_req++;
		send_data(fd, "received", MSG_SIZE);
	}
	else
	{
		send_data(fd, "nb_req limit", MSG_SIZE);
	}
}

/*
** keep iterating through all select fds
*/

void	select_cycles(fd_set *master, fd_set *read_fds, int *fdmax, int lfd)
{
	int		i;

	while (1)
	{
		memcpy(read_fds, master, sizeof(*master));
		if (select(*fdmax + 1, read_fds, NULL, NULL, NULL) == -1)
			return ;
		i = 0;
		while (i <= *fdmax)
		{
			if (FD_ISSET(i, read_fds))
			{
				if (i == lfd)
				{
					select_accept(i, master, fdmax);
				}
				else
				{
					select_recv(i, master);
				}
			}
			i++;
		}
		exec_cmd(&g_reqq);
	}
}

int		main(int ac, char **av)
{
	int		listener;

	if (ac < 13)
		server_usage();
	parse_args(av);
	g_reqq = NULL;
	SELECT_VARS;
	FD_ZERO(&master);
	FD_ZERO(&read_fds);
	listener = create_socket(g_env.port, 1);
	if (listen(listener, 42) == -1)
		return (EXIT_FAILURE);
	FD_SET(listener, &master);
	fdmax = listener;
	select_cycles(&master, &read_fds, &fdmax, listener);
	return (0);
}

