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

#include <sys/time.h>
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
** new added
** check msg recv from players has cmd inside
** if the cmd is not in the beginning of str -> false
** if cmd is not take, put, and broadcast, but have additional msg -> false
** if cmd is take and put, but the msg is not any of the resources -> false
** if cmd is broadcast, but doesn't have any additional msg -> false
*/

int		check_valid_cmd(char *msg, char *msg_buf, int i)
{
	// int				i;
	char			*tmp;
	unsigned long	len;

	// i = 0;
	printf("msg: |%s|%lu|\n", msg, strlen(msg));

	while (g_cmd[i].cmd)
	{
		if (!(tmp = strstr(msg, g_cmd[i].cmd)))
		{
			i++;
		}

		// else if (tmp != msg)
		// 	return (15);
		else
		{
			if ((strlen(msg) != (len = strlen(g_cmd[i].cmd))))
			{
				if (i != 5 && i != 6 && i != 8)
					return (16);
				else
				{
					if (i == 8)
						strcpy(msg_buf, msg + len + 1);
					else if (check_resource(strcpy(msg_buf, msg + len + 1)) == 7)
						return (17);
					return (i);
				}
			}
			// printf(LIGHTBLUE"\n\nstrlen(msg): |%lu|,msg: |%s|\n\n"RESET,strlen(msg),msg);
			// if (i != 5 && i != 6 && i != 8)
			// 	return (i);
			// else
			// 	return (18); // no text or object behind, only broadcast
			return ((i != 5 && i != 6 && i != 8) ? i : 19);
		}
	//	printf("\n\n{{{ I am checking the valid cmd }}}\n\n");
	}
	return (18);
}

void	record_time(t_event *node, int delay_time)
{
	struct timeval	exec_time;

	gettimeofday(&exec_time, NULL);

	exec_time.tv_sec += (exec_time.tv_usec + delay_time * g_env.ms_pre_tick) / 1000000;
	exec_time.tv_usec = (exec_time.tv_usec + delay_time * g_env.ms_pre_tick) % 1000000;
	node->exec_time = exec_time;
}

void	set_block_time(int fd)
{
	struct timeval	block_time;

	gettimeofday(&block_time, NULL);

	g_players[fd].block_time.tv_sec = block_time.tv_sec;
	g_players[fd].block_time.tv_usec = block_time.tv_usec;
	printf(LIME"\n[Finished set Block time]\n"RESET);
}

t_event	*init_event_node(int fd, char *msg, int delay_time, char *cmd)
{
	t_event			*node;

	node = (t_event *)malloc(sizeof(t_event));

	bzero(node, sizeof(t_event));
	node->fd = fd;
    bzero(node->cmd, CMD_LEN);
	strcpy(node->cmd, cmd);
	bzero(node->msg, MAX_MSG);
	strcpy(node->msg, msg);
	record_time(node, delay_time);
	if (!strcmp(msg, "fork") || !strcmp(msg, "incantation"))
		set_block_time(fd);
	node->next = NULL;
    printf("\n|node->fd %d|\n", node->fd);
	return (node);
}

void	insert(t_event *node)
{
	t_event	*tmp;

	if (!(g_env.queue_head))
		g_env.queue_head = node;
	else if (node->exec_time.tv_sec * 1000000 + node->exec_time.tv_usec <
			g_env.queue_head->exec_time.tv_sec * 1000000 +
			g_env.queue_head->exec_time.tv_usec)
	{
		node->next = g_env.queue_head;
		g_env.queue_head = node;
	}
	else
	{
		tmp = g_env.queue_head;
		while (tmp->next &&
			   node->exec_time.tv_sec * 1000000 + node->exec_time.tv_usec >
			   tmp->next->exec_time.tv_sec * 1000000 +
			   tmp->next->exec_time.tv_usec)
		{
			tmp = tmp->next;
		}
		node->next = tmp->next;
		tmp->next = node;
	}
}

void	enqueue(int fd, char *msg)
{
	char	msg_buf[1024];
	int		i;

	i = 0;
	bzero(msg_buf, 1024);
	i = check_valid_cmd(msg, msg_buf, 0);
	printf(YELLOW"cmd_index: [%d], msg: {%s}\n"RESET, i, msg);
	if (i == 9 || i == 10)
	{
		if (i == 9 && !cmd_incantation_check(fd))
		{
			send_data(fd, RED"INCANTATION KO"RESET, MSG_SIZE);
			return ;
		}
		g_players[fd].block = 1;
	}
	if (i == 11)
		cmd_connect_nbr(fd, msg);
	else
		insert(init_event_node(fd, msg_buf, g_cmd[i].delay_time, g_cmd[i].cmd));
	printf("request nb: %d\n", g_players[fd].request_nb);
	print_queue();
}

void	print_queue(void)
{
	t_event			*event;
	int				i;
	struct timeval	now;

	gettimeofday(&now, NULL);
	printf("event curr.tv_usec < %d >\n", now.tv_usec);
	printf("event curr.tv_sec < %ld >\n", now.tv_sec);
	i = 0;
	event = g_env.queue_head;
	if (event)
	{
		while (event)
		{
			printf("--------EVENT-----------\n");
			printf("event fd < %d >\n", event->fd);
			printf("event cmd < %s >\n", event->cmd);
			printf("event msg < %s >\n", event->msg);
			printf("event exec_time.tv_usec < %d >\n", event->exec_time.tv_usec);
			printf("event exec_time.tv_sec < %ld >\n", event->exec_time.tv_sec);
			event = event->next;
			printf("|event index: %d|\n", i++);
			printf("--------EVENT-----------\n");
		}
	}
	else
		printf(RED"\n----- [ queue is empty ] -- \n"RESET);
}

// void	exec_event(t_event **event, t_event **prev, t_event **h, t_event **l)
void	exec_event(void)
{
	int				i;
	struct timeval	now;

	if (!g_env.queue_head)
		return ;

	i = 0;
	gettimeofday(&now, NULL);
	if (check_event_time(&now, &g_env.queue_head->exec_time))
	{
		while (i < 13)
		{
			if (!strcmp(g_cmd[i].cmd, g_env.queue_head->cmd))
			{
				if (g_players[g_env.queue_head->fd].alive)
				{
					g_cmd[i].func(g_env.queue_head->fd, g_env.queue_head->msg);
					printf(LIGHTBLUE"\n[EXEC]\n"RESET);
				}
				// else
				// 	printf(LIGHTBLUE"\n[Player %d Dead, so no EXEC]\n"RESET, (*event)->fd);
				g_env.queue_head = g_env.queue_head->next;
				break ;
			}
			i++;
		}

	}
	// g_env.queue_head = g_env.queue_head->next;
	// printf("[cmd_ind after exec %i]\n", i);
}

// void	exec_event_and_delete(t_event **event, t_event **prev)
// {
// 	t_event			*tmp;
//
// 	exec_event(event);
// 	tmp = *event;
// 	if (!(*prev))
// 	{
// 		g_env.queue_head = g_env.queue_head->next;
// 		*event = (*event)->next;
// 	}
// 	else if (!(*event)->next)
// 	{
// 		(*prev)->next = NULL;
// 		*event = NULL;
// 	}
// 	else
// 	{
// 		*event = (*event)->next;
// 		(*prev)->next = *event;
// 	}
// 	free(tmp);
// }
//
// void	exec_event_list(void)
// {
// 	struct timeval	curr_time;
// 	t_event			*event;
// 	t_event			*prev;
//
// 	event = g_env.queue_head;
// 	prev = NULL;
// 	while (event)
// 	{
// 		gettimeofday(&curr_time, NULL);
//
// 		if (check_event_time(&curr_time, &(event->exec_time)) &&
// 			((!g_players[event->fd].block) ||
// 			check_event_time(&(event->exec_time),
// 			&(g_players[event->fd].block_time))))
// 		{
// 			//printf(RED"\n---------------------Here should be exec and pop from queue-----------------------\n");
// 			exec_event_and_delete(&event, &prev);
// 		}
// 		else
// 		{
// 			prev = event;
// 			event = event->next;
// 		}
// 	}
// }



void	cycle_exec_event_loop(void)
{
	exec_event();
	generate_resource();
	check_dead_player();
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
