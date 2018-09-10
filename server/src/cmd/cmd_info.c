/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_info.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sding <sding@student.42.us.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/07 17:40:56 by sding             #+#    #+#             */
/*   Updated: 2018/09/07 17:40:58 by sding            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/server.h"

int		cmd_info(int fd, char *msg)
{
	char	*str;

	(void)msg;
	printf("\n\n| [[[[[[[[cmd_info function]]]]] |\n\n");
	printf(CYAN"\n[Exec INFO]\n"RESET);
	printf(BLUE"Player [%d] -> [%s]\n"RESET, fd, "info");
	bzero(g_env.buffer, 4096);
	strcpy(g_env.buffer, RED);
	strcat(g_env.buffer, "[player ");
	strcat(g_env.buffer, (str = ft_itoa(fd)));
	free(str);
	strcat(g_env.buffer, " info: ]\n");
	strcat(g_env.buffer, "team_name: ");
	strcat(g_env.buffer, g_teams[g_players[fd].team_id].team_name);
	strcat(g_env.buffer, "\n");
	strcat(g_env.buffer, "y: ");
	strcat(g_env.buffer, (str = ft_itoa(g_players[fd].y)));
	free(str);
	strcat(g_env.buffer, " x: ");
	strcat(g_env.buffer, (str = ft_itoa(g_players[fd].x)));
	free(str);
	strcat(g_env.buffer, " direction: ");
	strcat(g_env.buffer, (str = ft_itoa(g_players[fd].direction)));
	free(str);
	strcat(g_env.buffer, "\n");
	strcat(g_env.buffer, "request_nb: ");
	strcat(g_env.buffer, (str = ft_itoa(g_players[fd].request_nb)));
	free(str);
	strcat(g_env.buffer, "\n");
	strcat(g_env.buffer, "level: ");
	strcat(g_env.buffer, (str = ft_itoa(g_players[fd].level)));
	free(str);
	strcat(g_env.buffer, "\n");
	strcat(g_env.buffer, "dead: ");
	strcat(g_env.buffer, (str = ft_itoa(g_players[fd].dead)));
	free(str);
	strcat(g_env.buffer, "\n");
	strcat(g_env.buffer, "block: ");
	strcat(g_env.buffer, (str = ft_itoa(g_players[fd].block)));
	free(str);
	strcat(g_env.buffer, "\n");
	strcat(g_env.buffer, "alive: ");
	strcat(g_env.buffer, (str = ft_itoa(g_players[fd].alive)));
	free(str);
	strcat(g_env.buffer, "\n");
	// live and block_time
	strcat(g_env.buffer, RESET);
	if (send_msg(fd, g_env.buffer, "Send [INFO]") == EXIT_FAILURE)
		return (EXIT_FAILURE);
	g_players[fd].request_nb--;
	printf(CYAN"\n[INFO success]\n"RESET);
	return (EXIT_SUCCESS);
}
