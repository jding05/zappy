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

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <netdb.h>
# include <errno.h>
# include <sys/select.h>

# define BUF_SIZE32 32
// # define MAX_FD 42
// # define MAX_TEAM 4
# define MAX_TEAM_NAME 32

# define SOCKET_VARS struct addrinfo hints, *ai; struct protoent *proto;
# define SELECT_VARS fd_set master, read_fds; int fdmax;

// # define WELCOME_MSG "WELCOME 🙂\n"
// # define TEAM_FULL_MSG "TEAM IS FULL\nBYE 😕\n"


# include "../../inc/server.h"
# include "../../inc/prototype.h"
//
// void	recv_print(int fd)
// {
// 	char	buf[BUF_SIZE];
// 	int		nbytes;
//  	if ((nbytes = recv(fd, buf, BUF_SIZE - 1, 0)) > 0)
// 	{
// 		buf[nbytes] = '\0';
// 		if (*buf == '#')
// 		{
// 			write(2, &buf[1], nbytes);
// 		}
// 		else
// 			write(1, buf, nbytes);
// 	}
// 	else
// 	{
// 		perror("recv error\n");
// 		memset(buf, 0, nbytes);
// 	}
//
// }

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

int		s_create_socket(char *port, int reuse)
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
	char					buf[BUF_SIZE32];
	int						nbytes;
	char					remote_ip[INET6_ADDRSTRLEN];
	int		tbytes;
	char	msg[BUF_SIZE32];
	int		i;

	addrlen = sizeof(remoteaddr);
	if ((newfd = accept(fd, (struct sockaddr *)&remoteaddr, &addrlen)) == -1)
		perror("accept");
	s_send_msg(newfd, WELCOME);


	tbytes = 0;
	while (tbytes < BUF_SIZE32)
	{
		nbytes = recv(newfd, buf, BUF_SIZE32, 0);
		if (nbytes < 0)
		{
			perror("recv error\n");
			printf("\n\n nbytes < 0 \n\n");
			return ;
		}
		if (nbytes == 0)
		{
			printf("recv nothing\n");
		}
		buf[nbytes] = '\0';
		tbytes += nbytes;
		i = 0;
		while (i < BUF_SIZE32)
		{
			if (buf[i] == '#')
				buf[i] = '\0';
			i++;
		}
		strncpy(msg, buf, i);
		memset(buf, 0, BUF_SIZE32);
		if (tbytes == BUF_SIZE32)
		{
			break ;
		}
	}
	if (s_add_to_team(msg, newfd) == EXIT_FAILURE)
	{
		printf("\n\n\n< fail to add into the team >\n\n\n");
		close(newfd);
	}

	else
	{
		printf("\n\n error after add to team \n\n");
		FD_SET(newfd, master);
		if (newfd > *fdmax)
			*fdmax = newfd;
		printf("\n\n error here \n\n");
		printf("new connection from %s on socket %d\n",
				inet_ntop(remoteaddr.ss_family,
					get_in_addr((struct sockaddr*)&remoteaddr),
					remote_ip, INET6_ADDRSTRLEN), newfd);
	}
	printf("\n\n\n{ FINISH ACCEPT}\n\n\n");
}

/*
** if a fd != listener, i.e. it's already connected to the server,
** receive the buffer then store the data into structs.
*/

// void	s_select_recv(int fd, fd_set *master)
// {
// 	char	buf[BUF_SIZE];
// 	int		nbytes;
//
// 	if ((nbytes = recv(fd, buf, BUF_SIZE, 0)) <= 0)
// 	{
// 		if (nbytes == 0)
// 		{
// 			// s_init_new_player(fd);
// 			printf("Player on socket %d left\n", fd);
// 		}
// 		else
// 		{
// 			printf("-->>>>> receive error\n");
// 			perror(strerror(errno));
// 		}
// 		close(fd);
// 		FD_CLR(fd, master);
// 	}
// 	else
// 	{
// 		if (g_players[fd].request_nb < 11)
// 		{
// 			buf[nbytes] = '\0';
// 			printf("request_nb = %d\n", g_players[fd].request_nb);
// 			printf("%d bytes received: |%s|\n", nbytes, buf);
// 			enqueue(fd, buf); // -> need to check *************************
// 			g_players[fd].request_nb++;
// 			printf("%d bytes received: |%s|\n", nbytes, buf);
// 			memset(buf, 0, strlen(buf));
// 			// s_send_msg(fd, "received\n");
// 			s_send_msg(fd, "received");
// 		}
// 		else
// 		{
// 			// s_send_msg(fd, "#request limit exceeded\n");
// 			s_send_msg(fd, "nb_req limit");
// 		}
// 	}
// }
void	s_select_recv(int fd, fd_set *master)
{
	int		nbytes;
	int		tbytes;
	char	buf[BUF_SIZE32];
	char	msg[BUF_SIZE32];
	int		i;

	tbytes = 0;
	while (tbytes < BUF_SIZE32)
	{
		nbytes = recv(fd, buf, BUF_SIZE32, 0);

		if (nbytes <= 0)
		{
			if (nbytes == 0)
			{
				// s_init_player(fd);
				g_players[fd].dead = 1;
				g_players[fd].alive = 0;
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
			while (i < BUF_SIZE32)
			{
				if (buf[i] == '#')
					buf[i] = '\0';
				i++;
			}
			strncpy(msg, buf, i);
			memset(buf, 0, BUF_SIZE32);
			if (tbytes == BUF_SIZE32)
			{
				printf("server recv msg = |%s|\n", msg);
				break ;
			}
		}
	}
	if (g_players[fd].request_nb < 11)
	{
		// buf[nbytes] = '\0';
		printf("nb_req = %d\n", g_players[fd].request_nb);
		// enqueue(&g_cmdq, fd, msg);
		enqueue(fd, msg);
		g_players[fd].request_nb++;
		printf("%d bytes received: |%s|\n", nbytes, msg);
		memset(buf, 0, BUF_SIZE32);
		s_send_msg(fd, "received");
	}
	else
		s_send_msg(fd, "nb_req limit");
}

/*
** keep iterating through all select fds
*/

void	s_select_cycles(fd_set *master, fd_set *read_fds, int *fdmax, int lfd)
{
	int		i;

	while (1)
	{
		printf("[Start s_select_cycle]\n");
		memcpy(read_fds, master, sizeof(*master));
		if (select(*fdmax + 1, read_fds, NULL, NULL, NULL) == -1)
			return ;
		i = -1;
		while (++i <= *fdmax)
		{
			if (FD_ISSET(i, read_fds))
			{
				if (i == lfd)
					s_select_accept(i, master, fdmax);
				else
					s_select_recv(i, master);
			}
		}
		cycle_exec_event_loop();
		// if (!g_env.queue_head)
		// 	printf(RED"\n{ event queue empty }\n"RESET);
		// else
		// 	print_queue();
		if (check_winner())
			break ;
		printf("[Finish s_select_cycle]\n");
	}
}

int		setup_socket(void)
{
	int		listener;

	printf("[Start Setup_socket]\n");
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
