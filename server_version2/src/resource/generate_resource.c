/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   generate_resource.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sding <sding@student.42.us.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/15 22:34:04 by sding             #+#    #+#             */
/*   Updated: 2018/09/15 22:34:05 by sding            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/server.h"

/*
** for a team to win the game, there should be at least 6 players reach level 8
** -> for 6 players to reach max level, the min requirement resources, total:
** 	 -> linemate 54, deraumere 48, sibur 60, mendiane 30, phiras 36, thystame 6
**
** -> Total min resource should be 234, and the portion of each resources are:
** -> linemate 23%, deraumere 19%, sibur 26%
**	  mendiane 13%, phiras 15%, thystame 3%
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
	if (rand_nb < 23)
		res = 1;
	else if (rand_nb < (23 + 19))
		res = 2;
	else if (rand_nb < (23 + 19 + 26))
		res = 3;
	else if (rand_nb < (23 + 19 + 26 + 13))
		res = 4;
	else if (rand_nb < (23 + 19 + 26 + 13 + 15))
		res = 5;
	else if (rand_nb < (23 + 19 + 26 + 13 + 15 + 3))
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

void		generate_resource(void)
{
	int		res;
	int		food;
	int		y;
	int		x;

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
}
