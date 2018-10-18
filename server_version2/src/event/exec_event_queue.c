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

static void	hatching_final()
{
	t_hatch	*tmp;
	int		id;

	id = g_env.hatching->team_id;
	printf(CYAN"\n[Exec HATCH]\n"RESET);
	g_teams[id].egg[g_teams[id].egg_hatched].hatched = 1;
	g_teams[id].egg_hatched++;
	g_teams[id].nb_client++;
	g_env.eggs--;
	printf(CYAN"\n[HATCH SUCCESS]\n"RESET);
	tmp = g_env.hatching;
	g_env.hatching = g_env.hatching->next;
	free(tmp);
}

void		exec_event(void)
{
	int				i;
	int				j;
	struct timeval	now;
	int				count;
	char	*gfx_data;

	generate_resource();
	gettimeofday(&now, NULL);
	j = 4;
	count = 0;
	while (++j < MAX_FD && count < g_env.active)
	{
		i = -1;
		if (g_players[j].alive && !g_players[j].dead && g_players[j].cstart != g_players[j].cend)
		{
			if (check_event_time(&now, &(g_players[j].cbuff[g_players[j].cstart].exec_time)))
			{
				i = g_players[j].cbuff[g_players[j].cstart].i;
				g_cmd[i].func(j, g_players[j].cbuff[g_players[j].cstart].msg,
						g_players[j].cbuff[g_players[j].cstart].player_id);
				printf("[player %d] is exec cmd\n", j);
				gfx_data = get_gfx_data();
				if (g_env.gfx_fd > 0)
					send_data(g_env.gfx_fd, gfx_data, MSG_SIZE);
				free(gfx_data);
				g_players[j].cstart = (g_players[j].cstart + 1) % 10;
				count++;
			}
		}
		if (g_env.eggs > 1 && g_env.hatching != NULL &&
		check_event_time(&now, &(g_env.hatching->exec_time)))
			hatching_final();
	}
}
