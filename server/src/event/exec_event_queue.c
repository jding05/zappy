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
** when we execute the ndoe, we dequeue,
** but with the blocking, we cannot exec the node,
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
	i = -1;
	gettimeofday(&now, NULL);
	if (check_event_time(&now, &(g_env.queue_head->exec_time)))
	{
		while (++i < 13)
		{
			if (!strcmp(g_cmd[i].cmd, g_env.queue_head->cmd))
			{
				if (g_players[g_env.queue_head->fd].alive)
				{
					g_cmd[i].func(g_env.queue_head->fd, g_env.queue_head->msg);
					gfx_data = get_gfx_data();
					if (g_env.gfx_fd > 0)
						send_data(g_env.gfx_fd, gfx_data, MSG_SIZE);
					generate_resource();
					free(gfx_data);
				}
				tmp = g_env.queue_head;
				g_env.queue_head = g_env.queue_head->next;
				free(tmp);
				break ;
			}
		}
	}
}
