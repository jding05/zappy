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

/*
** exec_event_queue()
**   1. get the current time
**   2. get the first node event in queue
**   3. check if any event exist in the queue
**      and check if the event execute time is right now or already passed
**   4. if check_event_time() return 1 -> we need to execute the command
**      else  return and wait for the next cycle;
**   5. make a loop to check the cmd and execute
**			no matter the cmd is valid cmd or not, the event node will be pop
**			and update the current time and check the event time again.
**			-> if the event time is not ready to execute, then stop, else continue
*/

void	exec_event_queue(void)
{
	struct timeval	curr_time;
	t_event			*event;
	t_event			*tmp;
	int				i;

	gettimeofday(&curr_time, NULL);
	event = peek_queue(g_env->queue);
	if (!event || !check_event_time(&curr_time, event->exec_time))
		return ;
	while (event)
	{
		while (g_cmd[i])
			(!strcmp(g_cmd[i], event->msg)) ? g_cmd[i].func() : i++;
		tmp = event;
		event = event->next;
		pop_queue(tmp);
		gettimeofday(&curr_time, NULL);
		!check_event_time(&curr_time, event->exec_time) ? break : continue;
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
