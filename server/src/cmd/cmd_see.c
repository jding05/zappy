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
	strcpy(g_env.buffer, BRED"{");
	while (floor < level + 1)
	{
		x_start = x - floor;
		while (x_start < x_end)
		{
			if (flag)
				flag = 0;
			else
				strcat(g_env.buffer, ", ");
			print_cell_value(update_y(y), update_x(x_start));
			x_start++;
		}
		floor++;
		y--;
		x_end++;
	}
	strcat(g_env.buffer, BRED"}"RESET);
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
	strcpy(g_env.buffer, BRED"{");
	while (floor <= level)
	{
		x_start = x + floor;
		while (x_start > x_end)
		{

			if (flag)
				flag = 0;
			else
				strcat(g_env.buffer, ", ");
			print_cell_value(update_y(y), update_x(x_start));
			x_start--;
		}
		floor++;
		y++;
		x_end--;
	}
	strcat(g_env.buffer, BRED"}"RESET);
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
	strcpy(g_env.buffer, BRED"{");
	while (floor <= level)
	{
		y_start = y - floor;
		while (y_start < y_end)
		{
			if (flag)
				flag = 0;
			else
				strcat(g_env.buffer, ", ");
			print_cell_value(update_y(y_start), update_x(x));
			y_start++;
		}
		floor++;
		x++;
		y_end++;
	}
	strcat(g_env.buffer, BRED"}"RESET);
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
	strcpy(g_env.buffer, BRED"{");
	while (floor <= level)
	{
		y_start = y + floor;
		while (y_start > y_end)
		{
			if (flag)
				flag = 0;
			else
				strcat(g_env.buffer, ", ");
			print_cell_value(update_y(y_start), update_x(x));
			y_start--;
		}
		floor++;
		x--;
		y_end--;
	}
	strcat(g_env.buffer, BRED"}"RESET);
}

void	cmd_see(int fd, char *msg)
{
	(void)msg;
	printf(CYAN"\n[Exec SEE]\n"RESET);
	printf(BLUE"Player [%d] -> [%s]\n"RESET, fd, "see");
	printf("current level: %d\n", g_players[fd].level);
	printf("players %d, pos-> y: %d x: %d d: %d\n", fd, g_players[fd].y, g_players[fd].x, g_players[fd].direction);
	g_players[fd].request_nb--;
	bzero(g_env.buffer, MSG_SIZE);
	if (g_players[fd].direction == NORTH)
		see_north_area(g_players[fd].level, g_players[fd].y, g_players[fd].x);
	else if (g_players[fd].direction == SOUTH)
		see_south_area(g_players[fd].level, g_players[fd].y, g_players[fd].x);
	else if (g_players[fd].direction == WEST)
		see_west_area(g_players[fd].level, g_players[fd].y, g_players[fd].x);
	else if (g_players[fd].direction == EAST)
		see_east_area(g_players[fd].level, g_players[fd].y, g_players[fd].x);
	printf("players %d, pos-> y: %d x: %d d: %d\n", fd, g_players[fd].y, g_players[fd].x, g_players[fd].direction);
	printf(CYAN"\n[SEE SUCCESS]\n"RESET);
	// maybe update graphic client regarding player position
	send_data(fd, g_env.buffer, MSG_SIZE);
}
