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

#include "../../include/server.h"

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
	int		i;
	char	*id;

	i = -1;
	while (++i < MAX_FD)
	{
		if (g_players[i].y == y && g_players[i].x == x)
		{
			if (g_players[i].alive)
			{
				// printf("player%d ", i);
				strcat(g_env.buffer, "player");
				strcat(g_env.buffer, (id = ft_itoa(i)));
			}

		}
	}
}

/*
** print_resource() needs to change print to send,
**	 maybe save all the things to buffer
*/

void	print_resource(int y, int x)
{
	int	i;
	int	count;

	i = -1;
	// printf(" {%d,%d} ", y, x);
	while (++i < 7)
	{
		count = g_env.map[y][x][i];
		while (count > 0)
		{
			// printf(LIME"%s "RESET, g_res_name[i]);
			strcat(g_env.buffer, g_res_name[i]);
			strcat(g_env.buffer, " ");
			count--;
		}
	}
	find_cell_player(y, x);
	// printf(", ");
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
	int flag;

	flag = 1;
	floor = 0;
	x_end = x + 1;
	// printf("{");
	strcpy(g_env.buffer, RED"{");
	while (floor < level + 1)
	{
		x_start = x - floor;
		while (x_start < x_end)
		{
			// if (flag)
			// 	flag = 0;
			// else
			// 	printf(", ");
			// flag ? flag = 0 : printf(", ");
			// flag ? flag = 0 : strcat(g_env.buffer, ", ");
			if (flag)
				flag = 0;
			else
				strcat(g_env.buffer, ", ");
			print_cell_value(y, x_start);
			x_start++;
		}
		floor++;
		y--;
		x_end++;
	}
	// printf("}");
	strcat(g_env.buffer, "}"RESET);
}

/*
** see south area from the (y,x) coordinate, expand with the level
*/

void	see_south_area(int level, int y, int x)
{
	int floor;
	int x_end;
	int x_start;
	int	flag;

	flag = 1;
	floor = 0;
	x_end = x - 1;
	// printf("{");
	strcpy(g_env.buffer, RED"{");
	while (floor <= level)
	{
		x_start = x + floor;
		while (x_start > x_end)
		{
			// if (flag)
			// 	flag = 0;
			// else
			// 	printf(", ");
			// flag ? flag = 0 : printf(", ");
			// flag ? flag = 0 : strcat(g_env.buffer, ", ");
			if (flag)
				flag = 0;
			else
				strcat(g_env.buffer, ", ");
			print_cell_value(y, x_start);
			x_start--;
		}
		floor++;
		y++;
		x_end--;
	}
	// printf("}");
	strcat(g_env.buffer, "}"RESET);
}

/*
** see east area from the (y,x) coordinate, expand with the level
*/

void	see_east_area(int level, int y, int x)
{
	int floor;
	int y_end;
	int y_start;
	int	flag;

	flag = 1;
	floor = 0;
	y_end = y + 1;
	// printf("{");
	strcpy(g_env.buffer, RED"{");
	while (floor <= level)
	{
		y_start = y - floor;
		while (y_start < y_end)
		{
			// if (flag)
			// 	flag = 0;
			// else
			// 	printf(", ");
			// flag ? flag = 0 : printf(", ");
			// flag ? flag = 0 : strcat(g_env.buffer, ", ");
			if (flag)
				flag = 0;
			else
				strcat(g_env.buffer, ", ");
			print_cell_value(y_start, x);
			y_start++;
		}
		floor++;
		x++;
		y_end++;
	}
	// printf("}");
	strcat(g_env.buffer, "}"RESET);
}

/*
** see west area from the (y,x) coordinate, expand with the level
*/

void	see_west_area(int level, int y, int x)
{
	int	floor;
	int	y_end;
	int	y_start;
	int	flag;

	flag = 1;
	floor = 0;
	y_end = y - 1;
	// printf("{");
	strcpy(g_env.buffer, RED"{");
	while (floor <= level)
	{
		y_start = y + floor;
		while (y_start > y_end)
		{
			// if (flag)
			// 	flag = 0;
			// else
			// 	printf(", ");
			// flag ? flag = 0 : printf(", ");
			// flag ? flag = 0 : strcat(g_env.buffer, ", ");
			if (flag)
				flag = 0;
			else
				strcat(g_env.buffer, ", ");
			print_cell_value(y_start, x);
			y_start--;
		}
		floor++;
		x--;
		y_end--;
	}
	// printf("}");
	strcat(g_env.buffer, "}"RESET);
}

int		cmd_see(int fd, char *msg)
{
	(void)msg;
	printf(CYAN"\n[Exec SEE]\n"RESET);
	printf(BLUE"Player [%d] -> [%s]\n"RESET, fd, "see");
	printf("current level: %d\n", g_players[fd].level);
	printf("players %d, pos-> y: %d x: %d d: %d\n", fd, g_players[fd].y, g_players[fd].x, g_players[fd].direction);
	g_players[fd].request_nb--;
	bzero(g_env.buffer, MSG_SIZE);
	if (g_players[fd].direction == NORTH)
	{
		// printf("BEFORE player direction: N Y= |%i| X = |%i|\n", g_players[fd].y, g_players[fd].x);
		see_north_area(g_players[fd].level, g_players[fd].y, g_players[fd].x);
		// printf("AFTER player direction: N Y= |%i| X = |%i|\n", g_players[fd].y, g_players[fd].x);
	}
	else if (g_players[fd].direction == SOUTH)
	{
		// printf("player direction: S Y= |%i| X = |%i|\n", g_players[fd].y, g_players[fd].x);
		see_south_area(g_players[fd].level, g_players[fd].y, g_players[fd].x);
		// printf("AFTER player direction: S Y= |%i| X = |%i|\n", g_players[fd].y, g_players[fd].x);
	}

	else if (g_players[fd].direction == WEST)
	{
		// printf("player direction: W Y= |%i| X = |%i|\n", g_players[fd].y, g_players[fd].x);
		see_west_area(g_players[fd].level, g_players[fd].y, g_players[fd].x);
		// printf("AFTER player direction: W Y= |%i| X = |%i|\n", g_players[fd].y, g_players[fd].x);
	}
	else if (g_players[fd].direction == EAST)
	{
		// printf("player direction: E Y= |%i| X = |%i|\n", g_players[fd].y, g_players[fd].x);
		see_east_area(g_players[fd].level, g_players[fd].y, g_players[fd].x);
		// printf("AFTER player direction: E Y= |%i| X = |%i|\n", g_players[fd].y, g_players[fd].x);
	}
	printf("players %d, pos-> y: %d x: %d d: %d\n", fd, g_players[fd].y, g_players[fd].x, g_players[fd].direction);
	printf(CYAN"\n[SEE SUCCESS]\n"RESET);
	// maybe update graphic client regarding player position
	send_data(fd, g_env.buffer, MSG_SIZE);
	return (EXIT_SUCCESS);
}
