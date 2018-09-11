/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfeng <zfeng@student.42.us.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/04 16:34:11 by zfeng             #+#    #+#             */
/*   Updated: 2018/09/10 23:07:06 by zfeng            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"

t_env	g_env;
t_team	g_teams[MAX_TEAM];

int		get_num(char **argv, int i)
{
	return (atoi(argv[i + 1]));
}

void	set_tname(char **argv, int i)
{
	int		team_id;

	i++;
	team_id = 0;
	while (*argv[i] != '-')
	{
		g_teams[team_id].team_id = team_id;
		if (strlen(argv[i]) < MAX_TEAM_NAME)
			strcpy(g_teams[team_id].team_name, argv[i]);
		i++;
		team_id++;
	}
}

void	parse_args(char **argv)
{
	int		i;
	int		f;

	i = 1;
	while(argv[i])
	{
		if (*argv[i] == '-')
		{
			f = *(++argv[i]);
			if (f == 'p')
				strcpy(g_env.port, argv[i + 1]);
			else if (f == 'x')
				g_env.map_x = get_num(argv, i);
			else if (f == 'y')
				g_env.map_y = get_num(argv, i);
			else if (f == 'c')
				g_env.max_player = get_num(argv, i);
			else if (f == 't')
				g_env.tud = get_num(argv, i);
			else if (f == 'n')
				set_tname(argv, i);
		}
		i++;
	}	
	i = 0;
	while (*g_teams[i].team_name)
		i++;
	g_env.nb_team = i;
	i = 0;
	if (g_env.nb_team > 0)
	{
		g_env.nb_client = g_env.max_player / g_env.nb_team;
		while (*g_teams[i].team_name)
		{
			g_teams[i].team_id = i;
			g_teams[i].nb_client = g_env.nb_client;
			i++;
		}
	}
	else
		write(2, "useage\n", 7);
}

void	server_usage(void)
{
	printf("Usage: ./server -p <port> -x <width> -y <height> \
			-n <team> [<team>] [<team>] ... -c <nb> -t <t>\n");
	printf("-p port number\n-x world width\n-y world height\n");
	printf("-n team\\_name\\_1 team\\_name\\_2 ...\n");
	printf("-c number of clients authorized at the beginning of the game\n");
	printf("-t time unit divider \
			(the greater t is, the faster the game will go)\n");
}

/*
int		main(int argc, char **argv)
{
	int		i;

	parse_args(argv);
	printf("port = %s | x = %d | y = %d | max_player = %d | t = %d |"
			" nb_team = %d | nb_client = %d\n",
			g_env.port, g_env.map_x, g_env.map_y, g_env.max_player,
			g_env.tud, g_env.nb_team, g_env.nb_client);
	printf("team1 = %s | team2 = %s | team3 = %s\n",
			g_teams[0].team_name, g_teams[1].team_name,
			g_teams[2].team_name);
	printf("nb_client1 = %d | nb_client2 = %d | nb_client3 = %d\n",
			g_teams[0].nb_client, g_teams[1].nb_client, g_teams[2].nb_client);
	return (0);
}
*/
