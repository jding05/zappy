/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sding <sding@student.42.us.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/29 19:20:08 by sding             #+#    #+#             */
/*   Updated: 2018/08/29 19:20:09 by sding            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/server.h"

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

/*
** generate resource by randomize the map[y][x][res];
** method:
**  1. increase res 0 ~ 6
**  2. while loop (fixed number of set) square root of map_x
**  3. while loop (fixed width + height times)
*/

void	generate_resource(void)
{
	int		res;
	int		count;

	res = 0;
	printf(YELLOW"[Generate Resource]\n"RESET);
	count = (int)sqrt(g_env.map_x) * (g_env.map_x + g_env.map_y);
	while (count-- > 0)
		while (res < 7)
			g_env.map[rand() % g_env.map_y][rand() % g_env.map_x][res++]++;
}

int		check_winner(void)
{
	int		i;
	int		check;

	check = 0;
	i = -1;
	while (++i < g_env.nb_team)
	{
		if (g_teams[i].reach_max_level == 6)
		{
			printf(RED"[WINNER IS TEAM <%s>]\n"RESET, g_teams[i].team_name);
			check = 1;
		}
	}
	return (check == 1 ? 1 : 0);
}

void	check_dead_player(void)
{
	int				i;
	int				food;
	struct timeval	curr_time;

	i = -1;
	gettimeofday(&curr_time, NULL);
	while (++i < MAX_FD)
	{
		if (g_players[i].fd && !g_players[i].dead)
		{
			if ((food = g_players[i].inventory[0]) > 0)
				update_live(i, food);
			if (check_event_time(&curr_time, &(g_players[i].live)))
			{
				g_players[i].dead = 1;
				// clear_queue(i);
			}
		}
	}
}

/*
** rought select loop()
**  // the last param in select() need to change for the timeout
*/

// void	server_client_connection(void)
// {
// 	fd_set			*select_fds;
// 	int				i;
// 	struct timeval	*timeout;
// 	int				short_term;

// 	timeout = NULL; // need to fix the value here
// 	short_term = 1;
// 	while (select(FD_SETSIZE, &select_fds, NULL, NULL, timeout))
// 	{
// 		i = -1;
// 		while (++i < FD_SETSIZE)
// 			if (FD_ISSET(i, &select_fds))
// 				i == g_env.server_fd ? new_client() : handle_cmd(i);
// 		if (g_env.time_unit > 10)
// 		{
// 			exec_event_queue(short_term);
// 			exec_event_queue(!short_term);
// 		}
// 		else
// 		{
// 			exec_event_list(short_term);
// 			exec_event_list(short_term);
// 		}
// 		generate_resource();
// 		check_dead_players();
// 		// might involve timeout
// 		if (check_winner())
// 			break ;
// 	}
// }

void	cycle_exec_event_loop(void)
{
	int				short_term;

	short_term = 1;
	if (g_env.time_unit > 10)
	{
		exec_event_queue(short_term);
		exec_event_queue(!short_term);
	}
	else
	{
		exec_event_list(short_term);
		exec_event_list(short_term);
	}
	generate_resource();
	check_dead_player();
}

// void	server_client_connection(void)
// {
// 	fd_set			*select_fds;
// 	int				i;
// 	struct timeval	*timeout;

// 	timeout = NULL; // need to fix the value here
// 	while (select(FD_SETSIZE, &select_fds, NULL, NULL, timeout))
// 	{
// 		i = -1;
// 		while (++i < FD_SETSIZE)
// 			if (FD_ISSET(i, &select_fds))
// 				i == g_env.server_fd ? new_client() : handle_cmd(i);
// 		cycle_loop();
// 		// might involve timeout
// 		if (check_winner())
// 			break ;
// 	}
// }

void	free_malloc(void)
{
	return ;
}

/*
** first genereate the resource for the game start
** second  server_client_connect() // this will be a select loop
*/

void	zappy_game(void)
{
	printf(RED"[GAME START ...]\n"RESET);
	generate_resource();
	setup_socket();
	printf(RED"[GAME END ...]\n"RESET);
	free_malloc();
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

int		main(int argc, char **argv)
{
	// t_env   env;

	// bzero(&g_env, sizeof(t_env);
	if (argc < 13 || !read_flags(argc, argv, &g_env))
	{
		printf("reading flags\n");
		server_usage();
		return (0);
	}
	zappy_game();
	// print_flags(&g_env);
	return (0);
}
