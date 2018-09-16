/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfeng <zfeng@student.42.us.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/14 13:47:07 by zfeng             #+#    #+#             */
/*   Updated: 2018/09/14 14:21:06 by zfeng            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "client.h"

 
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
		ERROR("socket error");
	}
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	sin.sin_addr.s_addr = inet_addr(addr);
	if (connect(sock, (const struct sockaddr *)&sin, sizeof(sin)) == -1)
	{
		ERROR("connect error");
	}
	return (sock);
}

int		main(int ac, char **av)
{
	int		sock;
	int		i;
	int		nbytes;
	char	buf[MSG_SIZE];
	char	*msg;

	SELECT_VARS;
	if (ac < 5 || EXIT_FAILURE == parse_args(av))
	{
		client_usage();
		return (EXIT_FAILURE);
	}
	FD_ZERO(&master);
	FD_ZERO(&read_fds);
	if (EXIT_FAILURE == (sock = create_client(g_env.host, atoi(g_env.port))))
	{
		return (EXIT_FAILURE);
	}
	FD_SET(sock, &master);
	FD_SET(STDIN_FILENO, &master);
	fdmax = sock;
	if (NULL == (msg = recv_data(sock, MSG_SIZE)))	// recv WELCOME msg
		return (EXIT_FAILURE);
	printf("%s\n", msg);
	send_data(sock, g_env.team_name, MAX_TEAM_NAME);	// send team name
	if (NULL == (msg = recv_data(sock, MSG_SIZE)))	// recv joined team OR team is full
		return (EXIT_FAILURE);
	printf("%s\n", msg);
	if (strcmp(msg, TEAM_FULL) == 0 || strcmp(msg, NAME_NOT_FOUND) == 0)
		return (EXIT_FAILURE);
	while (1)
	{
		memcpy(&read_fds, &master, sizeof(master));
		if (select(fdmax + 1, &read_fds, NULL, NULL, 0) == -1)
		{
			ERROR("select error");
		}
		i = 0;
		while (i <= fdmax)
		{
			if (FD_ISSET(i, &read_fds))
			{
				if (i == STDIN_FILENO)
				{
					if (0 == (nbytes = read(STDIN_FILENO, buf, MSG_SIZE - 1)))		// read stdin request
						ERROR("read from stdin error");
					buf[nbytes - 1] = '\0';		// -1 to remove \n
					if (validate_req(buf) == EXIT_SUCCESS)
					{
						send_data(sock, buf, MSG_SIZE);		// send request
					}
					else
					{
						write(1, "invalid command\n", 16);
					}
				}
				else
				{
					if (NULL == (msg = recv_data(sock, MSG_SIZE)))
						ERROR("recv error");
					printf("%s\n", msg);
				}
			}
			i++;
		}

	}
	close(sock);
	return (0);
}
