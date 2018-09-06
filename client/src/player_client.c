/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_client.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfeng <zfeng@student.42.us.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/25 22:47:38 by zfeng             #+#    #+#             */
/*   Updated: 2018/09/05 23:34:40 by zfeng            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"

char	*g_cmds[] = {"advance", "right", "left", "see", "inventory", \
	"take", "put", "kick", "incantation", "fork", "connect_nbr", 0};

char	*g_objects[] = {"linemate", "deraumere", "sibur", "mendiane", \
	"phiras", "thystame", "food", 0};

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

int		validate_cmd_with_obj(char *str, char *cmd)
{
	int		i;
	int		n;

	i = 0;
	n = strlen(cmd);
	if (strnstr(str, cmd, n))
	{
		printf("str = %s | cmd = %s | %s\n", str, cmd, strnstr(str, cmd, n));
		if (strcmp(strnstr(str, cmd, n), str) == 0)
		{
			while (g_objects[i])
			{
				printf("%s\n", &(str[n + 1]));
				if (strcmp(&str[n + 1], g_objects[i]) == 0)
				{
					printf("%s\n", g_objects[i]);
					return (EXIT_SUCCESS);
				}
				i++;
			}
		}
	}
	return (EXIT_FAILURE);
}

int		validate_cmd(char *cmd)
{
	int		i;

	i = 0;
	while (g_cmds[i])
	{
		if (strcmp(g_cmds[i], cmd) == 0)
			return (EXIT_SUCCESS);
		if (validate_cmd_with_obj(cmd, g_cmds[i]) == EXIT_SUCCESS)
			return (EXIT_SUCCESS);
		i++;
	}
	/*
	i = 0;
	if (strnstr(cmd, "put", 3))
	{
		if (strcmp(strnstr(cmd, "put", 3), cmd) == 0)
		{
			write(1, "here\n", 5);
			while (g_objects[i])
			{
				if (strcmp(&cmd[4], g_objects[i]) == 0)
					return (EXIT_SUCCESS);
				i++;
			}
		}
	}
	i = 0;
	if (strnstr(cmd, "take", 4))
	{
		if (strcmp(strnstr(cmd, "take", 4), cmd) == 0)
		{
			while (g_objects[i])
			{
				if (strcmp(&cmd[5], g_objects[i]) == 0)
					return (EXIT_SUCCESS);
				i++;
			}
		}
	}
	*/
	return (EXIT_FAILURE);
}

int		recv_print(int fd)
{
	char	buf[BUF_SIZE];
	int		nbytes;

	if ((nbytes = recv(fd, buf, BUF_SIZE - 1, 0)) > 0)
	{
		buf[nbytes] = '\0';
		if (*buf == FAILURE_CHAR)
		{
			write(2, &buf[1], nbytes);
			return (EXIT_FAILURE);
		}
		else
		{
			write(1, buf, nbytes);
		}
	}
	else
	{
		perror("recv error\n");
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}


int		main(int ac, char **av)
{
	int		sock;
	int		nbytes;
	char	buf[BUF_SIZE];

	parse_cargs(av);
	sock = create_client(g_env.host, atoi(g_env.port));
	if (recv_print(sock) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	send(sock, g_env.team_name, strlen(g_env.team_name), 0);
	if (recv_print(sock) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	while (1)
	{
		nbytes = read(STDIN_FILENO, buf, BUF_SIZE - 1);
		buf[nbytes - 1] = '\0';
		if (validate_cmd(buf) == EXIT_SUCCESS)
		{
			printf("client side buf = |%s|\n", buf);
			send(sock, buf, strlen(buf), 0);
			//if (recv_print(sock) == EXIT_FAILURE)
			//	return (EXIT_FAILURE);
			if (recv_print(sock) == EXIT_FAILURE)
				return (EXIT_FAILURE);
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



