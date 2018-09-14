/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sock.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfeng <zfeng@student.42.us.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/31 14:54:00 by zfeng             #+#    #+#             */
/*   Updated: 2018/09/03 20:55:37 by zfeng            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//# define BUF_SIZE32 32
# include "../../include/server.h"

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
	char					remote_ip[INET6_ADDRSTRLEN];
	char					*team_name;

	addrlen = sizeof(remoteaddr);
	if ((newfd = accept(fd, (struct sockaddr *)&remoteaddr, &addrlen)) == -1)
		perror("accept");
	send_data(newfd, WELCOME, MSG_SIZE);
	team_name = recv_data(newfd, MAX_TEAM_NAME);
	if (s_add_to_team(team_name, newfd) == EXIT_FAILURE)
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

void	s_select_recv(int fd, fd_set *master)
{
	char	*req;

	if (!(req = recv_data(fd, MSG_SIZE)))
	{
		close(fd);
		FD_CLR(fd, master);
		return ;
	}
	if (g_players[fd].request_nb < 11)
	{
		send_data(fd, "received", MSG_SIZE);
		enqueue(fd, req);
		g_players[fd].request_nb++;
	}
	else
	{
		send_data(fd, "request_nb limit", MSG_SIZE);
	}
}

struct timeval *set_timeout_alarm(void)
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

/*
** keep iterating through all select fds
*/

void	s_select_cycles(fd_set *master, fd_set *read_fds, int *fdmax, int lfd)
{
	int				i;
	struct timeval	*timeout;


	while (1)
	{
		memcpy(read_fds, master, sizeof(*master));
		timeout = set_timeout_alarm();
		if (timeout)
			printf("timeout alarm usec: |%d| sec = |%ld|\n", timeout->tv_usec, timeout->tv_sec);
		if (select(*fdmax + 1, read_fds, NULL, NULL, timeout) == -1)
		{
			printf("\nselect error %s\n", strerror(errno));
			return ;
		}
		cycle_exec_event_loop();
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
		// if (g_env.queue_head)
		//cycle_exec_event_loop();
		if (check_winner())
			break ;
		// printf("[Finish s_select_cycle]\n");
	}
}

int		setup_socket(void)
{
	int		listener;

	// printf("[Start Setup_socket]\n");
	SELECT_VARS;
	FD_ZERO(&master);
	FD_ZERO(&read_fds);
	listener = s_create_socket(g_env.port_name, 1);
	if (listen(listener, 42) == -1)
		return (EXIT_FAILURE);
	FD_SET(listener, &master);
	fdmax = listener;
	printf("[Finish Setup_socket]\n");
	s_select_cycles(&master, &read_fds, &fdmax, listener);
	return (0);
}
