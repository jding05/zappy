/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   zappy_server.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfeng <zfeng@student.42.us.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/25 19:46:02 by zfeng             #+#    #+#             */
/*   Updated: 2018/08/10 14:03:55 by zfeng            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 4242
#define BACKLOG 10
#define BUFF_SIZE 1024

#include <errno.h>
#include <string.h>

typedef struct	s_packet
{
	int		size;
	char	data[1024];
}				t_packet;

void	server_usage(char *str)
{
	printf("Usage: %s <port>\n", str);
	exit(-1);
}

int		ft_perror(char *str)
{
	write(2, str, strlen(str));
	return (EXIT_FAILURE);
}

int		create_server(int port)
{
	int					sock;
	struct protoent		*proto;
	struct sockaddr_in	sin;
	int					reuse;

	proto = getprotobyname("tcp");
	if (proto == 0)
		return (EXIT_FAILURE);
	if ((sock = socket(PF_INET, SOCK_STREAM, proto->p_proto)) == -1)
		return (ft_perror("ERROR: Socket error\n"));
	printf("SUCCESS: Server socket created\n");
	reuse = 1;
	if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int)) < 0)
		return(ft_perror("ERROR: Can't set the reuse option on the socket\n"));
	bzero(&sin, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(sock, (const struct sockaddr *)&sin, sizeof(sin)) == -1)
		return (ft_perror("ERROR: Bind error\n"));
	printf("SUCCESS: Bind to port %d\n", port);
	if (listen(sock, BACKLOG) == -1)
		return (ft_perror("ERROR: Listen error\n"));
	printf("SUCCESS: Listening...\n");
	return (sock);
}

void	accept_connection(int sock)
{
	int					csock;
	struct sockaddr_in	cs_addr;
	socklen_t			cs_addrsize;
	int					r;
	t_packet			*buf;
	char				*cmd;
	char				*av[4];

	if ((csock = accept(sock, (struct sockaddr *)&cs_addr, &cs_addrsize)) < 0)
		ft_perror("ERROR: Accept error\n");
	printf("SUCCESS: Connection accepted from %s:%d\n", inet_ntoa(cs_addr.sin_addr),
			ntohs(cs_addr.sin_port));
	if (fork() == 0)
	{
		close(sock);
		buf = (t_packet*)malloc(sizeof(t_packet));
		while ((r = recv(csock, buf, 512, 0)) > 0)
		{
			//buf[r] = '\0';
			//write(1, buf, strlen(buf));
			//send(csock, buf, strlen(buf), 0);
			printf("r = |%d|\n", r);
			printf("data length = %lu data = |%s|\n", strlen(buf->data), buf->data);
			printf("size = |%d|\n", buf->size);
		}
		exit(EXIT_SUCCESS);
	}
	else
		close(csock);
}

int		main(int ac, char **av)
{
	int		port;
	int		sock;

	if (ac != 2)
		server_usage(av[0]);
	port = atoi(av[1]);
	if ((sock = create_server(port)) == EXIT_FAILURE)
		return (ft_perror("ERROR: Creating server failed\n"));
	while (1)
	{
		accept_connection(sock);
	}
	return (EXIT_SUCCESS);
}
