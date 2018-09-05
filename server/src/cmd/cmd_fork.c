/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_fork.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sding <sding@student.42.us.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/17 18:26:52 by sding             #+#    #+#             */
/*   Updated: 2018/08/17 18:26:53 by sding            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/server.h"

/*
** the unauthorized connection means the dead players
** once the egg 42/t hit, means it call this function
** -> then we call cmd_hatch put into queue (that marked 600/t after)
*/
int     cmd_fork(t_players players, char *msg)
{
    printf(BLUE"Player [%d] -> [%s]\n"RESET, players.fd, "fork");
    players.request_nb--;
	(void)msg;
    laid_egg(&players);
    players.block = 1;
	push_cmd_hatch(players.fd, msg);
	if (send_msg(players.fd, "OK", "Send [fork]") == EXIT_FAILURE)
		return (EXIT_FAILURE);
    return(EXIT_SUCCESS);
}

void	calc_time_spead(void)
{
	struct timeval	ret;

	ret.tv_usec = ((double)1 / g_env.time_unit) * 1000000;
	ret.tv_usec %= 1000000;
	ret.tv_sec = ((double)1 / g_env.time_unit);
	g_env.time_speed = ret;
}

void	init_live(int fd)
{
	struct timeval	curr_time;

	gettimeofday(&curr_time, NULL);
    gettimeofday(&(g_players[fd].live), NULL);
	g_players[fd].live.tv_sec = curr_time.tv_sec;
	g_players[fd].live.tv_usec = curr_time.tv_usec;
	update_live(fd, 10);
}

void	update_live(int fd, int food)
{
	g_players[fd].live.tv_sec += 126 * food * g_env.time_speed.tv_sec;
	g_players[fd].live.tv_usec += 126 * food * g_env.time_speed.tv_usec;
	g_players[fd].live.tv_sec += (g_players[fd].live.tv_usec) / 1000000;
	g_players[fd].live.tv_usec %= 1000000;
}

void	record_time(t_event *node, int delay_time)
{
	struct timeval	curr_time;
	struct timeval	*exec_time;

	gettimeofday(&curr_time, NULL);
    exec_time = NULL;
	exec_time->tv_sec = curr_time.tv_sec;
	exec_time->tv_usec = curr_time.tv_usec;

	exec_time->tv_sec += delay_time * g_env.time_speed.tv_sec;
	exec_time->tv_usec += delay_time * g_env.time_speed.tv_usec;
	exec_time->tv_sec += (exec_time->tv_usec) / 1000000;
	exec_time->tv_usec %= 1000000;

	node->exec_time = exec_time;
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
	node->next = NULL;
	return (node);
}

void    push_cmd_hatch(int fd, char *msg)
{
	t_event *event;

	event = g_env.lt_queue->last;
	event->next = init_event_node(fd, msg, g_cmd[12].delay_time, g_cmd[12].cmd);
	event = event->next;
	event->next = NULL;
	g_env.lt_queue->last = event;
}

void    laid_egg(t_players *players)
{
	int	id;

	id = players->team_id;
	g_teams[id].egg[g_teams[id].egg_laid].team_id = id;
	g_teams[id].egg[g_teams[id].egg_laid].egg_id = g_teams[id].egg_laid;
	g_teams[id].egg[g_teams[id].egg_laid].hatched = 0;
	g_teams[id].egg[g_teams[id].egg_laid].direction = rand() % 4;
	g_teams[id].egg[g_teams[id].egg_laid].father_fd = players->fd;
	g_teams[id].egg[g_teams[id].egg_laid].y = players->y;
	g_teams[id].egg[g_teams[id].egg_laid].x = players->x;
	g_teams[id].egg_laid++;
}
