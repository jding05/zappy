// #include "util.h"
// #include "data_structures.h"

# include <server.h>

char    *options[] = {"-p","-x","-y","-n","-c","-t"};

int		isnbr_str(char *str)
{
	int i;

	i = -1;
	while (str[++i] != '\0')
		if (!isdigit(str[i]))
			return (0);
	return (1);
}

int		set_value(char **flag, int i, int m, t_env *env)
{
	int number;

	if (m == 0 || m == 1 || m == 2 || m == 4 || m == 5)
	{
		if (isnbr_str(flag[i + 1]))
			number = atoi(flag[i + 1]);
		else
			return (0);
	}
	if (m == 0)
	{
		env->port = number;
		strcpy(env->port_name, flag[i + 1]);
	}
	if (m == 1)
		env->map_x = number;
	if (m == 2)
		env->map_y = number;
	if (m == 3)
		// env->teams = team_init(flag, i + 1, g_env.nb_team);
		if (!team_init(flag, i + 1, g_env.nb_team))
			return (0);
	if (m == 4)
		env->authorized_clients = number;
	if (m == 5)
		env->time_unit = number;
	return (1);
}

int		team_init(char **argv, int i, int nb_team)
{
	int ind;

	ind = -1;
	while (++ind < nb_team)
	{
		g_teams[ind].team_id = ind;
		if (strlen(g_teams[ind].team_name) > MAX_TEAM_NAME)
			return (0);
		strcpy(g_teams[ind].team_name, argv[i++]);
		g_teams[ind].max_players = g_env.authorized_clients / g_env.nb_team;

		// might not need it 
		g_teams[ind].reach_max_level = 0;
		// g_teams[ind].egg_used = 0;
		g_teams[ind].egg_hatched = 0;
		g_teams[ind].egg_laid = 0;
	}
	return (0);
}

//                         0    1   2    3    4    5
// char    *options[] = {"-p","-x","-y","-n","-c","-t"};
/*
**  asssume every flag follow by 1 arg, there will be 13, 
**  so index 7 (-n) + i = index 9 (-c) -> i = argc (13) - 11
*/

int		read_flags(int argc, char **argv, t_env *env)
{
	int i;
	int m;

	i = 1;
	m = 0;
	bzero(env, sizeof(t_env));
	env->nb_team = argc - 12;
	while (i < argc)
	{
		if (!strcmp(argv[i], options[m]) && argv[i + 1])
		{
			if (!set_value(argv, i, m, env))
				return (0);
			i += (m != 3) ? 2 : argc - 11;
			m++;
		}
		else
			return (0);
	}
	update_max_player_per_team();
	return (1);
}

void	print_team(void)
{
	int i;

	i = -1;
	while (++i < g_env.nb_team)
	{
		printf("team_id [%d]\n", g_teams[i].team_id);
		printf("team_name [%s]\n", g_teams[i].team_name);
		printf("connected_players [%d]\n", g_teams[i].connected_players);
		printf("max_players [%d]\n", g_teams[i].max_players);
		printf("reach_max_level [%d]\n", g_teams[i].reach_max_level);
		// printf("egg_used [%d]\n", g_teams[i].egg_used);
		printf("egg_used [%d]\n", g_teams[i].egg_hatched);
		printf("egg_used [%d]\n", g_teams[i].egg_laid);
		printf("--------------------------------------\n");
	}
}

void	update_max_player_per_team(void)
{
	int i;

	i = -1;
	while (++i < g_env.nb_team)
		g_teams[i].max_players = g_env.authorized_clients / g_env.nb_team;
}

void	print_flags(t_env *env)
{
	int i;

	i = -1;
	printf(LIGHTBLUE"[print flag]\n\n"RESET);
	printf("-p Port:%d\n", env->port);
	printf("-x width:%d\n-y height:%d\n", env->map_x, env->map_y);
	while (++i < g_env.nb_team)
		printf("-n Teams:%s\n", g_teams[i].team_name);
	printf("-c Max Clients:%d\n", env->authorized_clients);
	printf("-t Time:%d\n",env->time_unit);
	printf("-------------------------------\n");
}