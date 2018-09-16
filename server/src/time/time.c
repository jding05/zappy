/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sding <sding@student.42.us.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/15 21:54:26 by sding             #+#    #+#             */
/*   Updated: 2018/09/15 21:54:28 by sding            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/server.h"

void	record_time(t_event *node, int delay_time)
{
	struct timeval	exec_time;

	if (g_players[node->fd].block)
	{
		exec_time.tv_sec = g_players[node->fd].block_time.tv_sec;
		exec_time.tv_usec = g_players[node->fd].block_time.tv_usec;
		exec_time.tv_sec += (exec_time.tv_usec + delay_time * g_env.ms_pre_tick) / 1000000;
		exec_time.tv_usec = (exec_time.tv_usec + delay_time * g_env.ms_pre_tick) % 1000000;
		node->exec_time = exec_time;
	}
	else
	{
		gettimeofday(&exec_time, NULL);
		exec_time.tv_sec += (exec_time.tv_usec + delay_time * g_env.ms_pre_tick) / 1000000;
		exec_time.tv_usec = (exec_time.tv_usec + delay_time * g_env.ms_pre_tick) % 1000000;
		node->exec_time = exec_time;
	}
}

inline void	set_block_time(t_event *node, int fd)
{
	g_players[fd].block_time.tv_sec = node->exec_time.tv_sec;
	g_players[fd].block_time.tv_usec = node->exec_time.tv_usec;
	printf(LIME"\n[Finished set Block time]\n"RESET);
}

/*
** check_event_time()
**   return -> 1 if current time is bigger than exec_time  ->(we can exec event)
**   return -> 0 if current time is smaller than exec_time ->(we stop)
*/

int		check_event_time(struct timeval *curr_time, struct timeval *exec_time)
{
	long int x;

	x = (curr_time->tv_sec - exec_time->tv_sec) * 1000000
		+ (curr_time->tv_usec - exec_time->tv_usec);
	if (x >= 0)
		return (1);
	else
		return (0);
}

void	calc_time_spead(void)
{
	g_env.ms_pre_tick = 1000000 / g_env.time_unit;
	printf("\n|g_env.ms_pre_tick:%ld\n", g_env.ms_pre_tick);
}
