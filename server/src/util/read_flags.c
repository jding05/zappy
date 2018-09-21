/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_flags.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sding <sding@student.42.us.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/15 22:01:23 by sding             #+#    #+#             */
/*   Updated: 2018/09/15 22:01:25 by sding            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/server.h"

static void	update_max_player_per_team(void)
{
	int i;

	i = -1;
	while (++i < g_env.nb_team)
		g_teams[i].max_players = g_env.authorized_clients / g_env.nb_team;
}

/*
**                         0    1   2    3    4    5
** char    *g_options[] = {"-p","-x","-y","-n","-c","-t"};
**
**  asssume every flag follow by 1 arg, there will be 13,
**  so index 7 (-n) + i = index 9 (-c) -> i = argc (13) - 11
*/

int			read_flags(int argc, char **argv, t_env *env)
{
	int i;
	int m;

	i = 1;
	m = 0;
	bzero(env, sizeof(t_env));
	if ((env->nb_team = argc - 12) > MAX_TEAM || env->nb_team == 0)
		return (0);
	while (i < argc)
	{
		if (!strcmp(argv[i], g_options[m]) && argv[i + 1])
		{
			if (!set_value(argv, i, m, env))
				return (0);
			i += (m != 3) ? 2 : argc - 11;
			m++;
		}
		else
			return (0);
	}
	if (env->nb_team > env->authorized_clients)
		return (0);
	update_max_player_per_team();
	calc_time_spead();
	return (1);
}
