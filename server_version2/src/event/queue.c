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

static int	init_event_node(int fd, char *msg, int i)
{
	int		cend;
	// char	*str;

	cend = g_players[fd].cend;
	g_players[fd].cbuff[cend].fd = fd;
	g_players[fd].cbuff[cend].player_id = g_players[fd].player_id;
	g_players[fd].cbuff[cend].i = i;
	bzero(g_players[fd].cbuff[cend].cmd, CMD_LEN);
	strcpy(g_players[fd].cbuff[cend].cmd, g_cmd[i].cmd);
	bzero(g_players[fd].cbuff[cend].msg, MAX_MSG);
	// if (i == 12)
	// {
	// 	strcpy(g_players[fd].cbuff[cend].msg, (str = ft_itoa(g_players[fd].team_id)));
	// 	free(str);
	// }
	// else
		strcpy(g_players[fd].cbuff[cend].msg, msg);
	record_time(&(g_players[fd].cbuff[cend]), g_cmd[i].delay_time);
	// if (i == 10)
	// 	set_block_time(&(g_players[fd].cbuff[cend]), fd);
	return (cend);
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

static int		enqueue_norm(char *msg_buf, int fd, int i)
{
	int	cend;

	cend = init_event_node(fd, msg_buf, i);
	if (i == 9 && !cmd_incantation_check(&(g_players[fd].cbuff[cend])))
	{
		send_data(fd, RED"INCANTATION KO"RESET, MSG_SIZE);
		return (-1);
	}
	if (i == 10)
	{
		if (g_teams[g_players[fd].team_id].egg_enqueued >= EGG_MAX)
		{
			send_data(fd, RED"FORK KO"RESET, MSG_SIZE);
			return (-1);
		}
		g_teams[g_players[fd].team_id].egg_enqueued++;
		// g_players[fd].block = 1;
		g_players[fd].status = 2;
	}
	g_players[fd].cend = (g_players[fd].cend + 1) % 10;
	return (1);
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

int				enqueue(int fd, char *msg)
{
	char	msg_buf[MSG_SIZE];
	int		i;
	char	*gfx_data;

	i = 0;
	bzero(msg_buf, MSG_SIZE);
	if ((i = check_valid_cmd(msg, msg_buf, 0)) > 15)
		return (send_data(fd, RED"invalid command"RESET, MSG_SIZE));
	printf(YELLOW"cmd_index: [%d], msg: {%s}\n"RESET, i, msg);
	if (i == 11)
		cmd_connect_nbr(fd, msg, g_players[fd].player_id);
	else
	{
		if (enqueue_norm(msg_buf, fd, i) == -1)
			return (-1);
		else
		{
			gfx_data = get_gfx_data();
			if (g_env.gfx_fd > 0)
				send_data(g_env.gfx_fd, gfx_data, MSG_SIZE);
			free(gfx_data);
		}
	}
	return (1);
}
