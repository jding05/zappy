/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_info.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sding <sding@student.42.us.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/15 22:22:48 by sding             #+#    #+#             */
/*   Updated: 2018/09/15 22:22:49 by sding            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../include/server.h"

void	print_player_info(int fd)
{
	printf(" [player %d] ", fd);
	printf("team: %d\n", g_players[fd].team_id);
	printf("level: %d\n", g_players[fd].level);
	printf("dead: %d\n", g_players[fd].dead);
	printf("direction: %d\n", g_players[fd].direction);
	printf("pos y: %d\n", g_players[fd].y);
	printf("pos x: %d\n", g_players[fd].x);
	printf("request_nb: %d\n", g_players[fd].request_nb);
	printf("-----------------------------\n");
}

void	print_team(void)
{
	int i;

	i = -1;
	while (++i < g_env.nb_team)
	{
		printf("team_id [%d]\n", g_teams[i].team_id);
		printf("team_name [%s]\n", g_teams[i].team_name);
		printf("cplayers [%d]\n", g_teams[i].cplayers);
		printf("max_players [%d]\n", g_teams[i].max_players);
		printf("reach_max_level [%d]\n", g_teams[i].reach_max_level);
		printf("egg_used [%d]\n", g_teams[i].egg_hatched);
		printf("egg_used [%d]\n", g_teams[i].egg_laid);
		printf("--------------------------------------\n");
	}
}

void	print_flags(void)
{
	int i;

	i = -1;
	printf(LIGHTBLUE"[print flag]\n\n"RESET);
	printf("-p Port:%s\n", g_env.port_name);
	printf("-x width:%d\n-y height:%d\n", g_env.map_x, g_env.map_y);
	while (++i < g_env.nb_team)
		printf("-n Teams:%s\n", g_teams[i].team_name);
	printf("-c Max Clients:%d\n", g_env.authorized_clients);
	printf("-t Time:%d\n", g_env.time_unit);
	printf("-------------------------------\n");
}
