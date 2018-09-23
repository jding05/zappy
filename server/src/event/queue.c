/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sding <sding@student.42.us.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/20 15:52:09 by sding             #+#    #+#             */
/*   Updated: 2018/09/20 15:52:10 by sding            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/server.h"

static t_event	*init_event_node(int fd, char *msg, int delay_time, char *cmd)
{
	t_event	*node;
	char	*str;

	node = (t_event *)malloc(sizeof(t_event));
	bzero(node, sizeof(t_event));
	node->fd = fd;
	node->player_id = g_players[fd].player_id;
	bzero(node->cmd, CMD_LEN);
	strcpy(node->cmd, cmd);
	bzero(node->msg, MAX_MSG);
	if (!strcmp(cmd, "hatch"))
	{
		strcpy(node->msg, (str = ft_itoa(g_players[fd].team_id)));
		free(str);
	}
	else
		strcpy(node->msg, msg);
	record_time(node, delay_time);
	if (!strcmp(cmd, "fork"))
		set_block_time(node, node->fd);
	node->next = NULL;
	return (node);
}

/*
** [ insert ] for priority queue,
** 	  3 cases:
**		1. there is nothing in the queue -> insert the node into queue
**		2. if the node has bigger priority than the head_node
**			-> insert before head_node, set itself as the head_node
**		3. if the node has less priority than the head_node, iterate through the
**			queue till find the lower priority ndoe than the current node,
**			-> insert right before
*/

static void		insert(t_event *node)
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
				node->exec_time.tv_sec * 1000000 + node->exec_time.tv_usec >=
				tmp->next->exec_time.tv_sec * 1000000 +
				tmp->next->exec_time.tv_usec)
		{
			tmp = tmp->next;
		}
		node->next = tmp->next;
		tmp->next = node;
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

static int		check_valid_cmd(char *msg, char *msg_buf, int i)
{
	char			*tmp;
	unsigned long	len;

	printf("msg: |%s|%lu|\n", msg, strlen(msg));
	while (g_cmd[i].cmd)
	{
		if (!(tmp = strstr(msg, g_cmd[i].cmd)))
			i++;
		else
		{
			if ((strlen(msg) != (len = strlen(g_cmd[i].cmd))))
			{
				if (i != 5 && i != 6 && i != 8)
					return (16);
				if (i == 8)
					strcpy(msg_buf, msg + len + 1);
				else if (check_resource(strcpy(msg_buf, msg + len + 1)) == 7)
					return (17);
				return (i);
			}
			return ((i != 5 && i != 6 && i != 8) ? i : 19);
		}
	}
	return (18);
}

/*
** [ enqueue ] the priority queue
** 		first. check which cmd index it is from check_valid_cmd()
**			-> in order to parse the right cmd
** 		second. if cmd is <connect_nbr>, since it is 0/t delay
**			-> send back the value immediately
** 		third. else, create the node first, to get the executing_time/block_time
**				then check if cmd <incantation> meet the requirement,
**				if the cmd is <fork> or <incantation>
**				-> block the players that are involved, those players cannot
**				execute the other cmd which send after the incantation start
**		last. insert the node in priority queue to build an event engine
*/

int			enqueue(int fd, char *msg)
{
	char	msg_buf[MSG_SIZE];
	int		i;
	t_event *node;
	char	*gfx_data;

	i = 0;
	bzero(msg_buf, MSG_SIZE);
	if ((i = check_valid_cmd(msg, msg_buf, 0)) > 15)
	{
		send_data(fd, RED"invalid command"RESET, MSG_SIZE);
		return (-1);
	}
	printf(YELLOW"cmd_index: [%d], msg: {%s}\n"RESET, i, msg);
	if (i == 11)
		cmd_connect_nbr(fd, msg, g_players[fd].player_id);
	else
	{
		node = init_event_node(fd, msg_buf, g_cmd[i].delay_time, g_cmd[i].cmd);
		if (i == 9 && !cmd_incantation_check(node))
		{
			send_data(fd, RED"INCANTATION KO"RESET, MSG_SIZE);
			free(node);
			return (-1);
		}
		if (i == 10)
		{
			if (g_teams[g_players[fd].team_id].egg_enqueued >= EGG_MAX)
			{
				send_data(fd, RED"FORK KO"RESET, MSG_SIZE);
				free(node);
				return (-1);
			}
			g_teams[g_players[fd].team_id].egg_enqueued++;
			g_players[fd].block = 1;
			g_players[fd].status = 2;
		}
		gfx_data = get_gfx_data();
		if (g_env.gfx_fd > 0)
			send_data(g_env.gfx_fd, gfx_data, MSG_SIZE);
		free(gfx_data);
		insert(node);
	}
	printf("request nb: %d\n", g_players[fd].request_nb);
	print_queue();
	return(1);
}
