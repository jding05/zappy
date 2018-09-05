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

# define BUF_SIZE 256
# define MAX_FD 16
# define MAX_TEAM 4
# define MAX_TEAM_NAME 32

# define SOCKET_VARS struct addrinfo hints, *ai; struct protoent *proto;
# define SELECT_VARS fd_set master, read_fds; int fdmax;

# define WELCOME_MSG "WELCOME 🙂\n"
# define TEAM_FULL_MSG "TEAM IS FULL\nBYE 😕\n"

# include "../../inc/server.h"
# include "../../inc/prototype.h"
/*
** print out the error message then return failure
*/

int		perror_rv(char *errmsg)
{
	write(2, errmsg, strlen(errmsg));
	return (EXIT_FAILURE);
}

/*
** when a new player is connected, add it to a team
*/

void	s_add_to_team(char *team_name, int fd, int nb_client)
{
	int		i;

	i = 0;
	// printf("\n|nb_client = %d|\n", nb_client);
	// printf("\n|team_name = %s|\n", team_name);
	// printf("\n|*g_teams[i].team_name = %s|\n", g_teams[i].team_name);
	// printf("\n|g_teams[i].nb_client %d|\n", g_teams[i].nb_client );
	// while (*g_teams[i].team_name)
	// {
	// 	if (strcmp(g_teams[i].team_name, team_name) == 0)
	// 	{
	// 		if (g_teams[i].nb_client == 0)
	// 			return ;
	// 		g_teams[i].nb_client--;
	// 		return ;
	// 	}
	// 	i++;
	// }
	g_teams[i].team_id = i;
	strcpy(g_teams[i].team_name, team_name);
	g_teams[i].nb_client = nb_client;
	g_players[fd].fd = fd;
	g_players[fd].team_id = i;
	printf("\n|g_teams[i].nb_client = %d|\n", g_teams[i].nb_client);
}

/*
** reset a player's data when the player_client is terminated
*/

void	s_reset_player(int fd)
{
	g_players[fd].request_nb = 0;
	memset(g_players[fd].inventory, 0, 7);
	// memset(g_players[fd].pos, 0, 2);
	g_players[fd].y = 0;
	g_players[fd].x = 0;
	g_players[fd].level = 0;
}

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

// static void init_player(int fd)
// {
// 	g_players[newfd].fd = fd;
//
// }
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
	// int						i;

	addrlen = sizeof(remoteaddr);
	if ((newfd = accept(fd, (struct sockaddr *)&remoteaddr, &addrlen)) == -1)
		perror("accept");
	send(newfd, WELCOME_MSG, strlen(WELCOME_MSG), 0);
	if ((nbytes = recv(newfd, buf, BUF_SIZE, 0)) < 0)
		perror(strerror(errno));
	buf[nbytes] = '\0';
	bzero(&g_players[newfd], sizeof(t_players));
	// i = g_players[newfd].team_id;
	// s_add_to_team(buf, newfd, g_teams[i].max_players + g_teams[i].egg_hatched);
	s_add_to_team(buf, newfd, 3);
	// init_player(newfd);
	init_live(newfd);
	// printf("\n| team_id = %d|\n", g_players[newfd].team_id);
	// printf("\n| nb_client = %d|\n", g_teams[g_players[newfd].team_id].nb_client);
	if (g_teams[g_players[newfd].team_id].nb_client == 0)
	{
		send(newfd, TEAM_FULL_MSG, strlen(TEAM_FULL_MSG), 0);
		close(newfd);
	}
	else
	{
		FD_SET(newfd, master);
		if (newfd > *fdmax)
			*fdmax = newfd;
		printf("%d\n", g_teams[g_players[newfd].team_id].nb_client);
		printf("x: | y: \n");
		send(newfd, "joined ", 7, 0);
		printf("selectserver: new connection from %s on socket %d\n",
				inet_ntop(remoteaddr.ss_family,
					get_in_addr((struct sockaddr*)&remoteaddr),
					remote_ip, INET6_ADDRSTRLEN), newfd);
	}
}

// void handle_msg(char *buf)
// {
// 	/* code */
// }

/*
** if a fd != listener, i.e. it's already connected to the server,
** receive the buffer then store the data into structs.
*/

void	s_select_recv(int fd, fd_set *master)
{
	char	buf[BUF_SIZE];
	int		nbytes;

	if ((nbytes = recv(fd, buf, BUF_SIZE, 0)) <= 0)
	{
		if (nbytes == 0)
		{
			s_reset_player(fd);
			printf("Player on socket %d left\n", fd);
		}
		else
			perror(strerror(errno));
		close(fd);
		FD_CLR(fd, master);
	}
	else
	{
		if (g_players[fd].request_nb < 11)
		{
			buf[nbytes] = '\0';
			printf("request_nb = %d\n", g_players[fd].request_nb);
			printf("%d bytes received: |%s|\n", nbytes, buf);
			enqueue(fd, buf); // -> need to check *************************
			g_players[fd].request_nb++;
			printf("%d bytes received: |%s|\n", nbytes, buf);
			memset(buf, 0, strlen(buf));
		}
	}
}

void print_queue(/* arguments */)
{
	t_event *event;

	event = g_env.st_queue->first;
	while (event)
	{
		printf("\n-------------------\nEVENT\n");
		printf("event fd < %d >\n", event->fd);
		printf("event cmd < %s >\n", event->cmd);
		printf("event msg < %s >\n", event->msg);
		event = event->next;
	}
}
/*
** keep iterating through all select fds
*/

void	s_select_cycles(fd_set *master, fd_set *read_fds, int *fdmax, int lfd)
{
	int		i;
	// int 	j = 0;

	while (1)
	{
	// while (j < 3)
	// {
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
		if (!g_env.st_queue->first)
		 	printf(RED"\nst_queue empty\n"RESET);
		else
			print_queue();
		if (!g_env.lt_queue->first)
			printf(RED"lt_queue empty\n"RESET);
		else
			print_queue();
		if (check_winner())
			break ;
	}
}

int		setup_socket(void)
{
	int		listener;

	SELECT_VARS;
	FD_ZERO(&master);
	FD_ZERO(&read_fds);
	listener = s_create_socket(g_env.port_name, 1);
	if (listen(listener, 42) == -1)
		return (EXIT_FAILURE);
	FD_SET(listener, &master);
	fdmax = listener;
	s_select_cycles(&master, &read_fds, &fdmax, listener);
	return (0);
}