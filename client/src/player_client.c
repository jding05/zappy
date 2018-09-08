/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_client.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfeng <zfeng@student.42.us.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/25 22:47:38 by zfeng             #+#    #+#             */
/*   Updated: 2018/09/07 13:35:03 by zfeng            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"

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
		perror(strerror(errno));
		return (EXIT_FAILURE);
	}
	return (sock);
}


int		main(int ac, char **av)
{
	int		sock;
	int		nbytes;
	char	buf[BUF_SIZE];

	parse_cargs(av);
	sock = create_client(g_env.host, atoi(g_env.port));

	recv_print(sock);
	
	send_msg(sock, g_env.team_name);

	recv_print(sock);

	while (1)
	{

		nbytes = read(STDIN_FILENO, buf, BUF_SIZE - 1);
		buf[nbytes - 1] = '\0';
		if (validate_cmd(buf) == EXIT_SUCCESS)
		{
			printf("client side buf = |%s|\n", buf);
			// send(sock, buf, strlen(buf), 0);
			send_msg(sock, buf);
			// if (recv_print(sock) == EXIT_FAILURE)
			// 	return (EXIT_FAILURE);
			recv_print(sock);
			memset(buf, 0, strlen(buf));
		}
		else
		{
			write(1, "invalid command\n", 16);
		}
	}
	close(sock);
	return (EXIT_SUCCESS);
}
