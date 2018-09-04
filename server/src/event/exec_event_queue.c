/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_event_queue.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sding <sding@student.42.us.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/17 15:40:45 by sding             #+#    #+#             */
/*   Updated: 2018/08/17 15:40:46 by sding            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/time.h>
#include "../../inc/server.h"

/*
** exec_event_queue()
**  -> this is applicable for the condition that only compare the current time
**     to the toppest event, if toppest event is not yet available to execute
**     stop the execute, w/o checking the events after which might be available
**	   to execute, this case, might block 1/t, due to some cmd has 7/t or 42/t
**   1. get the current time
**   2. get the first node event in queue
**   3. check if any event exist in the queue
**      and check if the event execute time is right now or already passed
**   4. if check_event_time() return 1 -> we need to execute the command
**      else  return and wait for the next cycle;
**   5. make a loop to check the cmd and execute
**			no matter the cmd is valid cmd or not, the event node will be pop
**			and update the current time and check the event time again.
**			-> if the event time is not ready to execute ->stop, else continue
*/

void	exec_event_queue(int short_term)
{
	struct timeval	curr_time;
	t_event			*event;
	t_event			*tmp;
	int				i;

	gettimeofday(&curr_time, NULL);
	event = short_term == 1 ? g_env.st_queue->first : g_env.lt_queue->first;
	if (!event || !check_event_time(&curr_time, event->exec_time))
		return ;
	while (event)
	{
		i = 0;
		while (g_cmd[i].cmd)
			(!strcmp(g_cmd[i].cmd, event->msg)) ? \
			g_cmd[i].func(g_players[event->fd], event->msg) : i++;
		tmp = event;
		event = event->next;
		free(tmp);
		gettimeofday(&curr_time, NULL);
		if (!check_event_time(&curr_time, event->exec_time))
			break ;
	}
}

void	exec_event(t_event **event, t_event **prev, t_event **h, t_event **l)
{
	t_event	*tmp;
	int		i;

	i = 0;
	while (g_cmd[i].cmd)
		(!strcmp(g_cmd[i].cmd, (*event)->msg)) ? \
		g_cmd[i].func(g_players[(*event)->fd], (*event)->msg) : i++;
	if (!(*prev))
	{
		*event = (*event)->next;
		tmp = *h;
		*h = *event;
		free(tmp);
	}
	else if (!(*event)->next)
	{
		tmp = *event;
		(*prev)->next = NULL;
		*l = *prev;
		free(tmp);
	}
	else
	{
		tmp = *event;
		(*prev)->next = (*event)->next;
		free(tmp);
	}
}

void	exec_event_list(int st_term)
{
	struct timeval	curr_time;
	t_event			*event;
	t_event			*head;
	t_event			*prev;
	t_event			*last;

	if (!(head = st_term == 1 ? g_env.st_queue->first : g_env.lt_queue->first)
	|| !(last = st_term == 1 ? g_env.st_queue->last : g_env.lt_queue->last))
		return ;
	prev = NULL;
	event = head;
	while (event)
	{
		gettimeofday(&curr_time, NULL);
		if (check_event_time(&curr_time, event->exec_time) && \
		!g_players[event->fd].block)
		{
			exec_event(&event, &prev, &head, &last);
			prev = event;
		}
		else
		{
			prev = event;
			event = event->next;
		}
	}
}

/*
** check_event_time()
**   return -> 1 if current time is bigger than exec_time  ->(we can exec event)
**   return -> 0 if current time is smaller than exec_time ->(we stop)
*/

int		check_event_time(struct timeval *curr_time, struct timeval *exec_time)
{
	if (curr_time->tv_sec > exec_time->tv_sec)
		return (1);
	else if (curr_time->tv_sec < exec_time->tv_sec)
		return (0);
	else
		return (curr_time->tv_usec > exec_time->tv_usec);
}
