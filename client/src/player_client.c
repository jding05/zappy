/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_client.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfeng <zfeng@student.42.us.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/25 22:47:38 by zfeng             #+#    #+#             */
/*   Updated: 2018/09/10 23:08:42 by zfeng            ###   ########.fr       */
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
	char	*msg;
	int		nbytes;
	char	buf[MSG_SIZE];
	char	*rv;

	parse_cargs(av);
	sock = create_client(g_env.host, atoi(g_env.port));
	msg = recv_data(sock, MSG_SIZE);	// recv WELCOME msg
	printf("%s\n", msg);
	memset(msg, 0, MSG_SIZE);
	send_data(sock, g_env.team_name, MAX_TEAM_NAME);	// send team name
	msg = recv_data(sock, MSG_SIZE);	// recv joined team OR team is full
	printf("%s\n", msg);
	if (strcmp(msg, TEAM_FULL) == 0 || strcmp(msg, NAME_NOT_FOUND) == 0)
		return (EXIT_FAILURE);
	memset(msg, 0, MSG_SIZE);
	while (1)
	{
		nbytes = read(STDIN_FILENO, buf, BUF_SIZE - 1);		// read stdin request
		buf[nbytes - 1] = '\0';  // -1 to remove \n
		if (validate_req(buf) == EXIT_SUCCESS)
		{
			send_data(sock, buf, MSG_SIZE);		// send request
			rv = recv_data(sock, MSG_SIZE);		// recv either received or exceed limit
			printf("%s\n", rv);
			memset(buf, 0, MSG_SIZE);
			rv = recv_data(sock, MSG_SIZE);		// recv return value from a command execution
			if (NULL != rv)
				printf("%s\n", rv);
			memset(rv, 0, MSG_SIZE);
		}
		else
		{
			write(1, "invalid command\n", 16);
		}

	}
	close(sock);
	return (EXIT_SUCCESS);
}
