
#include "server_client.h"

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
		//printf("ERROR: Connect error\n");
		perror(strerror(errno));
		return (EXIT_FAILURE);
	}
	return (sock);
}

int		validate_cmd(char *cmd)
{
	return (1);
}

void	recv_write(int sock)
{
	char	buf[BUF_SIZE];
	int		nbytes;

	if ((nbytes = recv(sock, buf, BUF_SIZE, 0)) > 0)
	{
		buf[nbytes] = '\0';
		write(1, buf, strlen(buf));
	}
	else
	{
		perror("recv");
	}
}


int		main(int ac, char **av)
{
	int		team_id;
	int		port;
	int		sock;
	int		nbytes;
	char	buf[BUF_SIZE];

	if (ac != 4)
		client_usage(av[0]);
	sock = create_client(av[3], atoi(av[2]));
	recv_write(sock);
	send(sock, av[1], strlen(av[1]), 0);
	if ((nbytes = recv(sock, buf, BUF_SIZE, 0)) > 0)
	{
		buf[nbytes] = '\0';	
		if (strcmp(buf, TEAM_FULL_MSG) == 0)
		{
			write(1, buf, strlen(buf));
			return (EXIT_FAILURE);
		}
		printf("%s %s\n", buf, av[1]);
	}
	while (1)
	{
		nbytes = read(STDIN_FILENO, buf, BUF_SIZE - 1);
		buf[nbytes - 1] = '\0';
		if (validate_cmd(buf))
		{
			printf("client side buf = |%s|\n", buf);
			send(sock, buf, strlen(buf), 0);
			memset(buf, 0, strlen(buf));
		}
	}
	close(sock);
	return (EXIT_SUCCESS);
}
