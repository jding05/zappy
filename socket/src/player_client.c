/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_client.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfeng <zfeng@student.42.us.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/25 22:47:38 by zfeng             #+#    #+#             */
/*   Updated: 2018/08/09 21:41:16 by zfeng            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFF_SIZE 1024

#include <errno.h>
#include <string.h>


typedef struct	s_packet
{
	int		size;
	char	data[1024];
}				t_packet;



void	client_usage(char *str)
{
	printf("Usage: %s <addr> <port>\n", str);
	exit(-1);
}

int		create_client(char *addr, int port)
{
	int					sock;
	struct protoent		*proto;
	struct sockaddr_in	sin;

	proto = getprotobyname("tcp");
	if (proto == 0)
		return (EXIT_FAILURE);
	if ((sock = socket(PF_INET, SOCK_STREAM, proto->p_proto)) == -1)
	{
		printf("ERROR: Socket error\n");
		return (EXIT_FAILURE);
	}
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	sin.sin_addr.s_addr = inet_addr(addr);
	if (connect(sock, (const struct sockaddr *)&sin, sizeof(sin)) == -1)
	{	
		printf("ERROR: Connect error\n");
		return (EXIT_FAILURE);
	}
	return (sock);
}

int		main(int ac, char **av)
{
	int		port;
	int		sock;
	int		r;
	t_packet	*buf;

	if (ac != 3)
		client_usage(av[0]);
	port = atoi(av[2]);
	sock = create_client(av[1], port);
//	while (1)
//	{
		//r = read(STDIN_FILENO, buf, BUFF_SIZE - 1);
		//buf[r] = '\0';
		//send(sock, buf, strlen(buf), 0);
	buf = (t_packet*)malloc(sizeof(t_packet));
	strcpy(buf->data, "hello world");
	printf("buf->data = |%s|\n", buf->data);
	buf->size = 11;
	printf("t_packet size = |%lu|\n", sizeof(t_packet));
	send(sock, buf, sizeof(t_packet), 0);
//	}
	close(sock);
	return (EXIT_SUCCESS);
}
