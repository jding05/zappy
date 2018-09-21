/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_value.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sding <sding@student.42.us.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/20 15:45:47 by sding             #+#    #+#             */
/*   Updated: 2018/09/20 15:45:50 by sding            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/server.h"

static int	isnbr_str(char *str)
{
	int i;

	i = -1;
	while (str[++i] != '\0')
	{
		if (!isdigit(str[i]))
			return (0);
	}
	return (1);
}

/*
** if the flag nb param is not fully number,
**	set the number to zero during set_value
** -> so if number = 0, it's not valid game environement variable
** -> m == 1 or m == 2 -> means maps y, x value,
**    we set the max map size is 20 * 20
*/

static int	check_flag_limit(int m, int number)
{
	if (m == 0 && (number < 1024 || number > 49151 || number <= 0))
	{
		printf("The server port range should be within 1024 to 49151\n");
		return (0);
	}
	if ((m == 1 || m == 2) && (number > MAP_XY || number <= 0))
	{
		printf("The server map size y or x value cannot exceed %d\n", MAP_XY);
		return (0);
	}
	if (m == 4 && (number > MAX_FD - 4 || number <= 0))
	{
		printf("The server authorized_client cannot exceed %d\n", (MAX_FD - 4));
		return (0);
	}
	if (m == 5 && (number > MAX_TIME_UNIT || number <= 0))
	{
		printf("The server time unit should be between 1 to %d\n",
		MAX_TIME_UNIT);
		return (0);
	}
	return (1);
}

static int	team_check_repeat(int nb_team)
{
	int	i;
	int	j;

	i = 0;
	while (i < nb_team)
	{
		j = i + 1;
		while (j < nb_team)
		{
			if (!strcmp(g_teams[i].team_name, g_teams[j].team_name))
				return (0);
			j++;
		}
		i++;
	}
	return (1);
}

static int	team_init(char **argv, int i, int nb_team)
{
	int ind;

	ind = -1;
	while (++ind < nb_team)
	{
		g_teams[ind].team_id = ind;
		if (strlen(g_teams[ind].team_name) > MAX_TEAM_NAME)
			return (0);
		strcpy(g_teams[ind].team_name, argv[i++]);
		g_teams[ind].nb_client = g_env.authorized_clients / g_env.nb_team;
		g_teams[ind].reach_max_level = 0;
		g_teams[ind].egg_hatched = 0;
		g_teams[ind].egg_laid = 0;
	}
	if (team_check_repeat(nb_team) == 0)
		return (0);
	return (1);
}

int			set_value(char **flag, int i, int m, t_env *env)
{
	int number;

	number = 0;
	if (m == 0 || m == 1 || m == 2 || m == 4 || m == 5)
		number = (isnbr_str(flag[i + 1])) ? atoi(flag[i + 1]) : 0;
	if (!check_flag_limit(m, number) && m != 3)
		return (0);
	if (m == 0)
		strcpy(env->port_name, flag[i + 1]);
	if (m == 1)
		env->map_x = number;
	if (m == 2)
		env->map_y = number;
	if (m == 3)
		if (!team_init(flag, i + 1, g_env.nb_team))
			return (0);
	if (m == 4)
		if ((env->authorized_clients = number) > MAX_FD - 4)
			return (0);
	if (m == 5)
		env->time_unit = number;
	return (1);
}
