/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_see.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sding <sding@student.42.us.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/30 15:56:58 by sding             #+#    #+#             */
/*   Updated: 2018/08/30 15:57:02 by sding            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/server.h"

/*
** haven't finished due to printf -> send buffer to player
*/

void	print_player_info(t_players players)
{
	printf(" [player %d] ", players.fd);
	printf("team: %d\n", players.team_id);
	printf("level: %d\n", players.level);
	printf("dead: %d\n", players.dead);
	printf("direction: %d\n", players.direction);
	printf("pos y: %d\n", players.y);
	printf("pos x: %d\n", players.x);
	printf("request_nb: %d\n", players.request_nb);
	printf("-----------------------------\n");
}

void	find_cell_player(int y, int x)
{
	int i;

	i = -1;
	while (++i < MAX_FD)
	{
		if (g_players[i].y == y && g_players[i].x == x)
		{
			if (!g_players[i].dead)
				printf(" [player %d] ", i);
		}
	}
}

/*
** print_resource() needs to change print to send,
**	 maybe save all the things to buffer
*/

char	*g_resource[] = \
{"food", "linemate", "deraumere", "sibur", "mendiane", "phiras", "thystame"};

void	print_resource(int y, int x)
{
	int	i;
	int	count;

	i = -1;
	while (++i < 7)
	{
		while ((count = g_env.map[y][x][i]) > 0)
		{
			printf(LIME"%s "RESET, g_resource[i]);
			count--;
		}
	}
	find_cell_player(y, x);
	printf(", ");
}

/*
** update the cell (y,x) coordinate
** -> due to the coordinate might over the board
** -> then call the print_resource function
**      -> to print the resource if exist, and player according to assign cell
*/

void	print_cell_value(int y, int x)
{
	if (y > g_env.map_y - 1)
		y = y - g_env.map_y;
	else if (y < 0)
		y = y + g_env.map_y;
	if (x > g_env.map_x - 1)
		x = x - g_env.map_x;
	else if (x < 0)
		x = x + g_env.map_x;
	print_resource(y, x);
}

/*
** see north area from the (y,x) coordinate, expand with the level
*/

void	see_north_area(int level, int y, int x)
{
	int	floor;
	int	x_end;
	int	x_start;

	floor = 0;
	x_end = x + 1;
	printf("{");
	while (floor < level + 1)
	{
		x_start = x - floor;
		while (x_start < x_end)
		{
			print_cell_value(y, x_start);
			x_start++;
		}
		floor++;
		y--;
		x_end++;
	}
	printf("}");
}

/*
** see south area from the (y,x) coordinate, expand with the level
*/

void	see_south_area(int level, int y, int x)
{
	int floor;
	int x_end;
	int x_start;

	floor = 0;
	x_end = x - 1;
	printf("{");
	while (floor <= level)
	{
		x_start = x + floor;
		while (x_start > x_end)
		{
			print_cell_value(y, x_start);
			x_start--;
		}
		floor++;
		y++;
		x_end--;
	}
	printf("}");
}

/*
** see east area from the (y,x) coordinate, expand with the level
*/

void	see_east_area(int level, int y, int x)
{
	int floor;
	int y_end;
	int y_start;

	floor = 0;
	y_end = y + 1;
	printf("{");
	while (floor <= level)
	{
		y_start = y - floor;
		while (y_start < y_end)
		{
			print_cell_value(y_start, x);
			y_start++;
		}
		floor++;
		x++;
		y_end++;
	}
	printf("}");
}

/*
** see west area from the (y,x) coordinate, expand with the level
*/

void	see_west_area(int level, int y, int x)
{
	int	floor;
	int	y_end;
	int	y_start;

	floor = 0;
	y_end = y - 1;
	printf("{");
	while (floor <= level)
	{
		y_start = y + floor;
		while (y_start > y_end)
		{
			print_cell_value(y_start, x);
			y_start--;
		}
		floor++;
		x--;
		y_end--;
	}
	printf("}");
}

int		cmd_see(t_players players, char *msg)
{
	(void)msg;
	printf(BLUE"Player [%d] -> [%s]\n"RESET, players.fd, "see");
	players.request_nb--;
	if (players.direction == NORTH)
		see_north_area(players.level, players.y, players.x);
	else if (players.direction == SOUTH)
		see_south_area(players.level, players.y, players.x);
	else if (players.direction == WEST)
		see_south_area(players.level, players.y, players.x);
	else if (players.direction == EAST)
		see_south_area(players.level, players.y, players.x);
	// maybe update graphic client regarding player position
	return (EXIT_SUCCESS);
}
