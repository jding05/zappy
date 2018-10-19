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

static void			zappy_game(void)
{
	printf(RED"[GAME START ...]\n"RESET);
	g_player_id = 1;
	memset(g_env.res, 0, sizeof(int) * 7);
	generate_resource();
	g_env.queue_head = NULL;
	setup_socket();
	printf(RED"[GAME END ...]\n"RESET);
}

static inline void	update_nb_client(void)
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

static void			server_usage(void)
{
	printf("Usage: ./server -p <port> -x <width> -y <height> ");
	printf("  -n <team> [<team>] [<team>] ... -c <nb> -t <t>\n");
	printf("  -p port number\n  -x world width\n  -y world height\n");
	printf("  -n team\\_name\\_1 team\\_name\\_2 ...\n");
	printf("  -c number of clients authorized at the beginning of the game\n");
	printf(
	"  -t time unit divider (the greater t is, the faster the game will go)\n");
}

int					main(int argc, char **argv)
{
	srand(time(NULL));
	bzero(&g_env, sizeof(t_env));
	if (argc < 13 || !read_flags(argc, argv, &g_env))
	{
		server_usage();
		return (1);
	}
	update_nb_client();
	zappy_game();
	if (g_env.gfx_fd > 0)
		send_data(g_env.gfx_fd, "GAME OVER@", MSG_SIZE);
	sleep(15);
	return (0);
}
