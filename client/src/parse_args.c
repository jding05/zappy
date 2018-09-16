/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfeng <zfeng@student.42.us.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/04 16:34:11 by zfeng             #+#    #+#             */
/*   Updated: 2018/09/10 23:08:36 by zfeng            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"

t_env	g_env;

int		get_num(char **argv, int i)
{
	return (atoi(argv[i + 1]));
}

void	client_usage(void)
{
	printf("Usage: ./client -n <team> -p <port> [-h <hostname>]\n");
	printf("-n team\\_name\n");
	printf("-p port\n");
	printf("-h name of the host, by default it'll be localhost\n");
}

int		parse_args(char **argv)
{
	int		i;
	int		f;

	i = 1;
	while (argv[i])
	{
		if (*argv[i] == '-')
		{
			f = *(++argv[i]);
			if (f == 'n')
				strcpy(g_env.team_name, argv[i + 1]);
			else if (f == 'p')
				strcpy(g_env.port, argv[i + 1]);
			else if (f == 'h')
				strcpy(g_env.host, argv[i + 1]);
		}
		i++;
	}
	if (!*g_env.host)
		strcpy(g_env.host, "127.0.0.1");
	if (!*g_env.team_name || !*g_env.port)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
