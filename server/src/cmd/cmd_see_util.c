/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_see_util.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sding <sding@student.42.us.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/15 22:12:32 by sding             #+#    #+#             */
/*   Updated: 2018/09/15 22:12:33 by sding            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/server.h"

static void	find_cell_player(int y, int x, int flag, int fd)
{
	int		i;
	char	*id;

	i = -1;
	while (++i < MAX_FD)
	{
		if (g_players[i].y == y && g_players[i].x == x && i != fd)
		{
			if (g_players[i].alive)
			{
				if (flag)
					flag = 0;
				else
					strcat(g_env.buffer, " ");
				strcat(g_env.buffer, CYAN);
				strcat(g_env.buffer, "player");
				strcat(g_env.buffer, (id = ft_itoa(i)));
				free(id);
				strcat(g_env.buffer, RESET);
			}
		}
	}
}

static void	print_resource(int y, int x, int fd)
{
	int	i;
	int	count;
	int flag;

	i = -1;
	flag = 1;
	strcat(g_env.buffer, GREY);
	while (++i < 7)
	{
		count = g_env.map[y][x][i];
		while (count > 0)
		{
			if (flag)
				flag = 0;
			else
				strcat(g_env.buffer, " ");
			strcat(g_env.buffer, g_res_name[i]);
			count--;
		}
	}
	find_cell_player(y, x, flag, fd);
}

int			update_y(int y)
{
	while (y < 0 || y >= g_env.map_y)
	{
		if (y > g_env.map_y - 1)
			y = y - g_env.map_y;
		else if (y < 0)
			y = y + g_env.map_y;
	}
	return (y);
}

int			update_x(int x)
{
	while (x < 0 || x >= g_env.map_x)
	{
		if (x > g_env.map_x - 1)
			x = x - g_env.map_x;
		else if (x < 0)
			x = x + g_env.map_x;
	}
	return (x);
}

/*
** update the cell (y,x) coordinate
** -> due to the coordinate might over the board
** -> then call the print_resource function
**      -> to print the resource if exist, and player according to assign cell
*/

void		print_cell_value(int y, int x, int fd)
{
	if (y > g_env.map_y - 1)
		y = y - g_env.map_y;
	else if (y < 0)
		y = y + g_env.map_y;
	if (x > g_env.map_x - 1)
		x = x - g_env.map_x;
	else if (x < 0)
		x = x + g_env.map_x;
	print_resource(y, x, fd);
}
