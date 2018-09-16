/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sding <sding@student.42.us.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/29 19:20:08 by sding             #+#    #+#             */
/*   Updated: 2018/08/29 19:20:09 by sding            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/server.h"

/*
** generate resource by randomize the map[y][x][res];
** method:
**  1. increase res 0 ~ 6
**  2. while loop (fixed number of set) square root of map_x
**  3. while loop (fixed width + height times)
*/

/*
** for a team to win the game, there should be at least 6 players reach level 8
** -> for 6 players to reach max level, the min requirement resources, total:
** 	 -> linemate 60, deraumere 60, sibur 60, mendiane 30, phiras 36, thystame 6
**
** -> Total min resource should be 246, and the portion of each resources are:
** -> linemate 22%, deraumere 24%, sibur 24% mendiane 12%, phiras 15%, thystame 2%
** -> the chance to generate the stone resource in one random cell,
**		it's the percentage value
** -> Total per stone resource should be [nb resource per stone * nb_team]
*/

static int	resource_dropping_rate(void)
{
	int		res;
	int		rand_nb;

	rand_nb = rand() % 100;
	res = 0;
	if (rand_nb < 22)
		res = 1;
	else if (rand_nb < (22 + 24))
		res = 2;
	else if (rand_nb < (22 + 24 + 24))
		res = 3;
	else if (rand_nb < (22 + 24 + 24 + 12))
		res = 4;
	else if (rand_nb < (22 + 24 + 24 + 12 + 15))
		res = 5;
	else if (rand_nb < (22 + 24 + 24 + 12 + 15 + 2))
		res = 6;
	return (res);
}

/*
**  [ generate resource ]
**		In each loop cycle, we first random generate the map y, x value,
**		to set the generate location. There are 7 resouces, 0 is food,
** 		rest of them is incantation stones, so we seperate the generation,
** Two things:
** 1. we generate food with 50% dropping rate, and if the current cell has less
** 		than 5 units of food (food max cap per cell), then we generate the food.
** 2. we call resource_dropping_rate() to get the resource id, that we need,
** 		for the 6 kinds of stone, we only generate one unit max (per stone )in
** 		each cell, if the cell doesn't have any unit of that stone, and the game
**		hasn't generated max stone units (which is the minimun stone requirement
** 		for one team to win * nb of teams), then we generate the stone.
*/

void	generate_resource(void)
{
	int		res;
	int		food;
	int		y;
	int		x;

	// printf(YELLOW"[Generate Resource]\n"RESET);
	y = rand() % g_env.map_y;
	x = rand() % g_env.map_x;
	res = resource_dropping_rate();
	if ((food = rand() % 100) < 50 && g_env.map[y][x][0] < 5)
		g_env.map[y][x][0]++;
	if (!g_env.map[y][x][res])
	{
		if (g_env.res[res] < g_max_res[res] * g_env.nb_team)
		{
			g_env.map[y][x][res]++;
			g_env.res[res]++;
		}
	}
	// printf("[Finish Generate]\n");
}

int		check_winner(void)
{
	int		i;
	int		check;
	int		fd;

	check = 0;
	fd = 1;
	i = -1;
	bzero(g_env.buffer, 4096);
	strcpy(g_env.buffer, YELLOW"\n\n[[[[YOUR TEAM HAS WON THE GAME ]]]]\n\n"RESET);
	while (++i < g_env.nb_team)
	{
		if (g_teams[i].reach_max_level == 6)
		{
			printf(YELLOW"\n\n[ WINNER IS TEAM < %s >]\n\n"RESET, g_teams[i].team_name);
			while (fd < MAX_FD && g_players[fd].alive)
			{
				if (g_players[fd].team_id == i)
				send_data(fd, g_env.buffer, MSG_SIZE);
			}
			check = 1;
		}
	}
	return (check == 1 ? 1 : 0);
}

void	check_dead_player(void)
{
	int				i;
	struct timeval	curr_time;

	i = -1;
	bzero(g_env.buffer, 4096);
	strcpy(g_env.buffer, DARKYELLOW"\n[ Your player is dead ]\n"RESET);
	gettimeofday(&curr_time, NULL);
	while (++i < MAX_FD)
	{
		if (g_players[i].fd && !g_players[i].dead)
		{
			if (check_event_time(&curr_time, &(g_players[i].live)))
			{
				g_players[i].dead = 1;
				g_players[i].alive = 0;
				send_data(i, g_env.buffer, MSG_SIZE);
			}
		}
	}
}

void	update_live(int fd, int nb_food)
{
	printf(BLUE"Player [%d] -> [Update %d live]\n"RESET, fd, nb_food);
	g_players[fd].live.tv_sec += (g_players[fd].live.tv_usec + nb_food * 126 * g_env.ms_pre_tick) / 1000000;
	g_players[fd].live.tv_usec = (g_players[fd].live.tv_usec + nb_food * 126 * g_env.ms_pre_tick) % 1000000;
}

void	free_malloc(void)
{
	return ;
}

inline void	init_res(void)
{
	memset(g_env.res, 0, sizeof(int) * 7);
}

inline void	init_queue(void)
{
	g_env.queue_head = NULL;
	printf("[init_queue]\n");
}

/*
** first genereate the resource for the game start
** second  server_client_connect() // this will be a select loop
*/

void	zappy_game(void)
{
	printf(RED"[GAME START ...]\n"RESET);
	init_res();
	generate_resource();
	init_queue();
	setup_socket();
	printf(RED"[GAME END ...]\n"RESET);
	free_malloc();
}

static void update_nb_client(void)
{
	int	ind;

	ind = -1;
	while (++ind < g_env.nb_team)
		g_teams[ind].nb_client = g_env.authorized_clients / g_env.nb_team;
}

/*
** Usage: ./server -p <port> -x <width> -y <height>
**            -n <team> [<team>] [<team>] ... -c <nb> -t <t>
**  1 -p port number
**  3 -x world width
**  5 -y world height
**  7 -n team\_name\_1 team\_name\_2 ...
**  8 + nb -c number of clients authorized at the beginning of the game
**  10 + nb -t time unit divider (the greater t is, the faster the game will go)
*/

void	server_usage(void)
{
	printf("Usage: ./server -p <port> -x <width> -y <height> ");
	printf("-n <team> [<team>] [<team>] ... -c <nb> -t <t>\n");
	printf("-p port number\n-x world width\n-y world height\n");
	printf("-n team\\_name\\_1 team\\_name\\_2 ...\n");
	printf("-c number of clients authorized at the beginning of the game\n");
	printf("-t time unit divider \
			(the greater t is, the faster the game will go)\n");
}

int		main(int argc, char **argv)
{
	bzero(&g_env, sizeof(t_env));
	if (argc < 13 || !read_flags(argc, argv, &g_env))
	{
		printf("reading flags\n");
		server_usage();
		return (0);
	}
	update_nb_client();
	zappy_game();
	// print_flags(&g_env);
	return (0);
}
