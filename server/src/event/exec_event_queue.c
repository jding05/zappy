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
			(!strcmp(g_cmd[i].cmd, event->cmd)) ? \
			g_cmd[i].func(g_players[event->fd], event->msg) : i++;
		tmp = event;
		event = event->next;
		free(tmp);
		gettimeofday(&curr_time, NULL);
		if (!check_event_time(&curr_time, event->exec_time))
			break ;
	}
}

void 	init_queue(void)
{
	g_env.st_queue = NULL;
	g_env.lt_queue = NULL;
	// g_env.st_queue->first = NULL;
	// g_env.st_queue->last = NULL;
	// g_env.lt_queue->first = NULL;
	// g_env.lt_queue->last = NULL;
}

void 	long_short_term(t_event *node, int short_term)
{
	if (short_term)
	{
		if (!(g_env.st_queue))
		{
			g_env.st_queue->first = node;
			g_env.st_queue->last = node;
			return ;
		}
		g_env.st_queue->last->next = node;
		g_env.st_queue->last = node;
	}
	else
	{
		if (!(g_env.st_queue))
		{
			g_env.st_queue->first = node;
			g_env.st_queue->last = node;
			return ;
		}
		g_env.st_queue->last->next = node;
		g_env.st_queue->last = node;
	}
}

/*
** new added
** check msg recv from players has cmd inside
** if the cmd is not in the beginning of str -> false
** if cmd is not take, put, and broadcast, but have additional msg -> false
** if cmd is take and put, but the msg is not any of the resources -> false
** if cmd is broadcast, but doesn't have any additional msg -> false
*/

int		check_valid_cmd(char *msg, char *msg_buf)
{
	int				i;
	char			*tmp;
	unsigned long	len;

	i = 0;
	while (g_cmd[i].cmd)
	{
		if (!(tmp = strstr(msg, g_cmd[i].cmd)))
			i++;
		else if (tmp != msg)
			return (13);
		else
		{
			if ((strlen(msg) != (len = strlen(g_cmd[i].cmd))))
			{
				if (i != 5 && i != 6 && i != 8)
					return (13);
				else
				{
					if (i == 8)
						strcpy(msg_buf, msg + len);
					else if (check_resource(strcpy(msg_buf, msg + len + 1)) == 7)
						return (13);
					return (i);
				}
			}
			if (i != 5 && i != 6 && i != 8)
				return (i);
		}
	}
	return (13);
}

void	enqueue(int fd, char *msg)
{
	t_event *node;
	char	msg_buf[1024];
	int		i;
	int		short_term;

	// bzero(msg, 1024);
	if ((i = check_valid_cmd(msg, msg_buf)) == 13)
	{
		// printf("\nenqueue : check invalid\n");
		return ;
	}
	printf("\nAM I wrong\n");
	printf("\n|cmd_ind %d|\n", i);
	printf("\n|msg_buf %s|\n", msg_buf);
	// node = init_event_node(fd, msg_buf, g_cmd[i].delay_time, g_cmd[i].cmd);
	node = init_event_node(fd, msg_buf, g_cmd[i].delay_time, g_cmd[i].cmd);
	short_term = 1;
	printf("\nAM I wrong\n");
	if (i == 9 || i == 12)
		long_short_term(node, !short_term);
	else
		long_short_term(node, short_term);
}

void	exec_event(t_event **event, t_event **prev, t_event **h, t_event **l)
{
	t_event	*tmp;
	int		i;

	i = 0;
	while (g_cmd[i].cmd)
		(!strcmp(g_cmd[i].cmd, (*event)->cmd)) ? \
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
