/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_event_queue.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sding <sding@student.42.us.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/17 15:40:45 by sding             #+#    #+#             */
/*   Updated: 2018/09/12 18:41:16 by zfeng            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/server.h"

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

/*
** when we execute the ndoe, we dequeue, but with the blocking, we cannot exec the node,
** -> so it will cause timeout alarm keep -> 0, and still won't be execute,
** need to change < the blocking or just dequeue one >
** xxx ** but fixed one player blocking issue
*/

void	exec_event(void)
{
	int				i;
	struct timeval	now;
	t_event			*tmp;
	char			*gfx_data;

	if (!g_env.queue_head)
		return ;
	i = 0;
	gettimeofday(&now, NULL);
	if (check_event_time(&now, &(g_env.queue_head->exec_time)))
	{
		while (i < 13)
		{
			if (!strcmp(g_cmd[i].cmd, g_env.queue_head->cmd))
			{
				if (g_players[g_env.queue_head->fd].alive)
				{
					g_cmd[i].func(g_env.queue_head->fd, g_env.queue_head->msg);
					printf(LIGHTBLUE"\n[EXEC]\n"RESET);
					gfx_data = get_gfx_data();
					printf("to gfx |%s|\n", gfx_data);
					if (g_env.gfx_fd > 0)
						send_data(g_env.gfx_fd, gfx_data, MSG_SIZE);
					free(gfx_data);
				}
				tmp = g_env.queue_head;
				g_env.queue_head = g_env.queue_head->next;
				free(tmp);
				break ;
			}
			i++;
		}
	}
}

void	cycle_exec_event_loop(void)
{
	exec_event();
	generate_resource();
	check_dead_player();
}
