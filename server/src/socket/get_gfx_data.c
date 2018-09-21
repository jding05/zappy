/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_gfx_data.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfeng <zfeng@student.42.us.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/17 16:15:32 by zfeng             #+#    #+#             */
/*   Updated: 2018/09/17 16:15:33 by zfeng            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/server.h"

char	*get_map_data(void)
{
	char	*map;
	char	*rv;
	int		i;
	int		j;

	if (NULL == (map = ft_strnew(g_env.map_x * g_env.map_y * 4 + 1)))
		return (NULL);
	i = -1;
	while (++i < g_env.map_y)
	{
		j = -1;
		while (++j < g_env.map_x)
		{
			rv = ft_itoa((g_env.map[i][j][0] > 0 ? 1 : 0) * pow(2, 6) +
			g_env.map[i][j][1] * pow(2, 5) + g_env.map[i][j][2] * pow(2, 4) +
			g_env.map[i][j][3] * pow(2, 3) + g_env.map[i][j][4] * pow(2, 2) +
			g_env.map[i][j][5] * pow(2, 1) + g_env.map[i][j][6]);
			strcat(map, rv);
			strcat(map, ",");
			free(rv);
		}
	}
	map[strlen(map) - 1] = '\0';
	strcat(map, "\n");
	return (map);
}

void	num_to_str(int num, char **str, char *delim)
{
	char	*rv;

	rv = ft_itoa(num);
	strcat(*str, rv);
	strcat(*str, delim);
	free(rv);
}

void	format_player_data(int i, char **players)
{
	int		j;

	num_to_str(g_players[i].player_id, players, ",");
	num_to_str(g_players[i].dead, players, ",");
	num_to_str(g_players[i].left, players, ",");
	num_to_str(g_players[i].level, players, ",");
	num_to_str(g_players[i].status, players, ",");
	if (g_players[i].status == 2)
		g_players[i].status = 0;
	num_to_str(g_players[i].team_id, players, ",");
	num_to_str(g_players[i].x, players, ",");
	num_to_str(g_players[i].y, players, ",");
	num_to_str(g_players[i].direction, players, ",");
	j = -1;
	while (++j < 6)
		num_to_str(g_players[i].inventory[j], players, ",");
	num_to_str(g_players[i].inventory[j], players, "\n");
}

char	*get_players_data(void)
{
	int		i;
	char	*players;

	i = -1;
	if (NULL == (players = ft_strnew(PLAYER_SIZE * MAX_FD + 1)))
		return (NULL);
	while (++i < MAX_FD)
	{
		if (0 != g_players[i].player_id)
			format_player_data(i, &players);
	}
	strcat(players, "@");
	return (players);
}

char	*get_eggs_data(void)
{
	int		i;
	int		j;
	char	*eggs;

	if (NULL == (eggs = ft_strnew(6 * MAX_TEAM * 6 + 1)))
		return (NULL);
	i = -1;
	while (++i < MAX_TEAM)
	{
		j = -1;
		while (++j < 6)
		{
			if (0 != *g_teams[i].team_name)
			{
				if (0 < g_teams[i].egg[j].ffd && 1 != g_teams[i].egg[j].hatched)
				{
					num_to_str(g_teams[i].egg[j].team_id, &eggs, ",");
					num_to_str(g_teams[i].egg[j].x, &eggs, ",");
					num_to_str(g_teams[i].egg[j].y, &eggs, "\n");
				}	
			}
			
		}
	}
	return (eggs);
}

char	*get_gfx_data(void)
{
	char	*map;
	char	*players;
	char	*eggs;
	char	*tmp;
	char	*gfx_data;

	map = get_map_data();
	eggs = get_eggs_data();
	players = get_players_data();
	tmp = ft_strjoin(map, eggs);
	gfx_data = ft_strjoin(tmp, players);
	free(map);
	free(eggs);
	free(players);
	free(tmp);
	return (gfx_data);
}
