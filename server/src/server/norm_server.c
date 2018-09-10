/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   norm_server.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfeng <zfeng@student.42.us.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/31 14:54:00 by zfeng             #+#    #+#             */
/*   Updated: 2018/09/08 18:15:55 by zfeng            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"
#include "parse.h"

t_player	g_players[MAX_FD];
t_team		g_teams[MAX_TEAM];
t_env		g_env;
t_cmdq		*g_cmdq;


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

int		s_iter_sock(struct addrinfo *ai, struct protoent *proto, int reuse)
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

int		s_create_socket(char* port, int reuse)
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
	listener = s_iter_sock(ai, proto, reuse);
	return (listener);
}

/*
** if a fd == listener, i.e. there is a new connection request, accept and
** connect
*/

void	s_select_accept(int fd, fd_set *master, int *fdmax)
{
	int						newfd;
	struct sockaddr_storage remoteaddr;
	socklen_t				addrlen;
	char					buf[BUF_SIZE];
	int						nbytes;
	char					remote_ip[INET6_ADDRSTRLEN];

	addrlen = sizeof(remoteaddr);
	if ((newfd = accept(fd, (struct sockaddr *)&remoteaddr, &addrlen)) == -1)
		perror("accept");

	
	send_msg(newfd, WELCOME);

	int		tbytes;
	char	msg[BUF_SIZE];
	int		i;

	tbytes = 0;
	while (tbytes < BUF_SIZE)
	{
		nbytes = recv(newfd, buf, BUF_SIZE, 0);
		if (nbytes <= 0)
		{
			perror("didn't receive team name\n");
			return ;
		}
		buf[nbytes] = '\0';
		tbytes += nbytes;
		i = 0;
		while (i < BUF_SIZE)
		{
			if (buf[i] == '#')
				buf[i] = '\0';
			i++;
		}
		strncpy(msg, buf, i);
		memset(buf, 0, BUF_SIZE);
		if (tbytes == BUF_SIZE)
		{
			break ;
		}
	}
	if (s_add_to_team(msg, newfd) == EXIT_FAILURE)
		close(newfd);	
	else
	{
		FD_SET(newfd, master);
		if (newfd > *fdmax)
			*fdmax = newfd;
		//printf("nb_client = %d\n", g_teams[g_players[newfd].team_id].nb_client);
		//printf("x: | y: \n");
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

void	s_select_recv(int fd, fd_set *master)
{
	int		nbytes;
	int		tbytes;
	char	buf[BUF_SIZE];
	char	msg[BUF_SIZE];
	int		i;

	tbytes = 0;
	while (tbytes < BUF_SIZE)
	{
		nbytes = recv(fd, buf, BUF_SIZE, 0);

		if (nbytes <= 0)
		{
			if (nbytes == 0)
			{
				// s_init_player(fd);
				s_reset_player(fd);
				printf("Player on socket %d left\n", fd);
			}
			else
			{
				perror("server recv error\n");
			}
			close(fd);
			FD_CLR(fd, master);
			return ;
		}	
		else
		{
			buf[nbytes] = '\0';
			tbytes += nbytes;
			i = 0;
			while (i < BUF_SIZE)
			{
				if (buf[i] == '#')
					buf[i] = '\0';
				i++;
			}
			strncpy(msg, buf, i);
			memset(buf, 0, BUF_SIZE);
			if (tbytes == BUF_SIZE)
			{
				printf("server recv msg = |%s|\n", msg);
				break ;
			}
		}
	}
	if (g_players[fd].nb_req < 11)
	{
		// buf[nbytes] = '\0';
		printf("nb_req = %d\n", g_players[fd].nb_req);
		enqueue(&g_cmdq, fd, msg);
		g_players[fd].nb_req++;
		printf("%d bytes received: |%s|\n", nbytes, msg);
		memset(buf, 0, BUF_SIZE);
		send_msg(fd, "received");
	}
	else
	{
		send_msg(fd, "nb_req limit");
	}

	// if ((nbytes = recv(fd, buf, BUF_SIZE, 0)) <= 0)
	// {
	// 	if (nbytes == 0)
	// 	{
	// 		s_init_player(fd);
	// 		printf("Player on socket %d left\n", fd);
	// 	}
	// 	else
	// 		perror(strerror(errno));
	// 	close(fd);
	// 	FD_CLR(fd, master);
	// }
	// else
	// {
	// 	if (g_players[fd].nb_req < 11)
	// 	{
	// 		buf[nbytes] = '\0';
	// 		printf("nb_req = %d\n", g_players[fd].nb_req);
	// 		enqueue(&g_cmdq, fd, buf);
	// 		g_players[fd].nb_req++;
	// 		printf("%d bytes received: |%s|\n", nbytes, buf);
	// 		memset(buf, 0, BUF_SIZE);
	// 		send_msg(fd, "received");
	// 	}
	// 	else
	// 	{
	// 		send_msg(fd, "nb_req limit");
	// 	}
	// }
}



/*
** keep iterating through all select fds
*/

void	s_select_cycles(fd_set *master, fd_set *read_fds, int *fdmax, int lfd)
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
					s_select_accept(i, master, fdmax);
				}
				else
				{
					s_select_recv(i, master);
				}
			}
			i++;
		}
		s_exec_cmd(&g_cmdq);
	}
}


int		main(int ac, char **av)
{
	int		listener;

	if (ac < 13)
		server_usage();
	parse_args(av);
	g_cmdq = NULL;
	SELECT_VARS;
	FD_ZERO(&master);
	FD_ZERO(&read_fds);
	listener = s_create_socket(g_env.port, 1);
	if (listen(listener, 42) == -1)
		return (EXIT_FAILURE);
	FD_SET(listener, &master);
	fdmax = listener;
	s_select_cycles(&master, &read_fds, &fdmax, listener);
	return (0);
}

