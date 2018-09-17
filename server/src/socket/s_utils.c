/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   s_utils.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfeng <zfeng@student.42.us.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/07 13:41:07 by zfeng             #+#    #+#             */
/*   Updated: 2018/09/08 17:17:39 by sding            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../include/server.h"

int		perror_rv(char *errmsg)
{
	write(2, errmsg, strlen(errmsg));
	return (EXIT_FAILURE);
}

char	*ft_strnew(size_t size)
{
	char	*new;

	new = (char*)malloc(sizeof(char) * (size + 1));
	memset(new, 0, size + 1);
	return (new);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*new;

	if (s1 && s2)
	{
		new = ft_strnew(strlen(s1) + strlen(s2));
		if (!new)
			return (NULL);
		strcpy(new, s1);
		strcat(new, s2);
		return (new);
	}
	return (NULL);
}

void	init_live(int fd)
{
	struct timeval	curr_time;

	gettimeofday(&curr_time, NULL);
    gettimeofday(&(g_players[fd].live), NULL);
	g_players[fd].live.tv_sec = curr_time.tv_sec;
	g_players[fd].live.tv_usec = curr_time.tv_usec;
	g_players[fd].block_time.tv_sec = curr_time.tv_sec;
	g_players[fd].block_time.tv_usec = curr_time.tv_usec;
	update_live(fd, 1000);
}

void	s_init_egg_player(int fd, int team_id, int egg_id)
{
	g_players[fd].fd = fd;
	g_players[fd].player_id = player_id++;
	g_players[fd].request_nb = 0;
	memset(g_players[fd].inventory, 0, sizeof(int) * 7);
	g_players[fd].y = g_teams[team_id].egg[egg_id].y;
	g_players[fd].x = g_teams[team_id].egg[egg_id].x;
	g_players[fd].request_nb = 0;
	g_players[fd].level = 1;
	g_players[fd].alive = 1;
	g_players[fd].dead = 0;
	g_players[fd].block = 0;
	g_players[fd].direction = rand() % 4;
	init_live(fd);
}

/*
** reset a player's data when the player_client is terminated
*/

void	s_init_new_player(int fd)
{
	static int	player_id = 0;

	g_players[fd].fd = fd;
	g_players[fd].player_id = player_id++;
	g_players[fd].request_nb = 0;
	memset(g_players[fd].inventory, 0, sizeof(int) * 7);
	g_players[fd].y = rand() % g_env.map_y;
	g_players[fd].x = rand() % g_env.map_x;
	g_players[fd].request_nb = 0;
	g_players[fd].level = 1;
	g_players[fd].alive = 1;
	g_players[fd].dead = 0;
	g_players[fd].block = 0;
	g_players[fd].direction = rand() % 4;
	init_live(fd);
}

/*
** when a new player is connected, add it to a team
*/

int		s_add_to_team(char *team_name, int fd)
{
	int		i;
	int		egg_id;
	char	*msg;

 	i = 0;
	while (*g_teams[i].team_name)
	{
		if (strcmp(g_teams[i].team_name, team_name) == 0)
		{
			if (g_teams[i].nb_client == 0)
			{
				send_data(fd, TEAM_FULL, MSG_SIZE);
				return (EXIT_FAILURE);
			}

			if ((egg_id = g_teams[i].max_players - g_teams[i].connected_players++) > 0)
				s_init_new_player(fd);
			else
				s_init_egg_player(fd, i, egg_id);
			g_players[fd].team_id = i;
			g_teams[i].nb_client--;
			// g_teams[i].connected_players++;
			msg = ft_strjoin("joined team ", team_name);
			send_data(fd, msg, MSG_SIZE);
			free(msg);
			return (EXIT_SUCCESS);
		}
		i++;
	}
	send_data(fd, TEAM_NOT_FOUND, MSG_SIZE);
	return (EXIT_FAILURE);
}

// char	*gen_map(void)
// {
// 	char	*map;
// 	char	*rv;
// 	int		i;
// 	int		j;
// 	int		bv;

// 	if (NULL == (map = (char*)malloc(sizeof(char) * (g_env.map_x * g_env.map_y * 4 + 7))))
// 		return (NULL);
// 	memset(map, 0, g_env.map_x * g_env.map_y * 4 + 7);
// 	rv = ft_itoa(g_env.map_x);
// 	strcpy(map, rv);
// 	strcat(map, ",");
// 	rv = ft_itoa(g_env.map_y);
// 	strcat(map, rv);
// 	strcat(map, "\n");
// 	i = 0;
// 	j = 0;
// 	while (i < g_env.map_x)
// 	{
// 		j = 0;
// 		while(j < g_env.map_y)
// 		{
// 			bv = g_env.map[i][j][0]*pow(2, 6) + 
// 			g_env.map[i][j][1]*pow(2, 5) + g_env.map[i][j][2]*pow(2, 4) + 
// 			g_env.map[i][j][3]*pow(2, 3) + g_env.map[i][j][4]*pow(2, 2) + 
// 			g_env.map[i][j][5]*pow(2, 1) + g_env.map[i][j][6];
// 			rv = ft_itoa(bv);
// 			strcat(map, rv);
// 			strcat(map, ",");
// 			j++;
// 		}
// 		i++;
// 	}
// 	map[strlen(map) - 1] = '\0';
// 	printf("%s\n", map);
// 	return (map);
// }