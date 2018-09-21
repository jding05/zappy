/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_broadcast_util.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xzhu <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/21 00:28:28 by xzhu              #+#    #+#             */
/*   Updated: 2018/09/21 00:28:38 by xzhu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/server.h"

void	send_broadcast_msg(int nb_dir, int fd, char *msg)
{
	bzero(g_env.buffer, MSG_SIZE);
	strcpy(g_env.buffer, RED"message ");
	strcat(g_env.buffer, ft_itoa(nb_dir));
	strcat(g_env.buffer, ",");
	strcat(g_env.buffer, msg);
	strcat(g_env.buffer, RESET);
	printf(RED"player %d, %s\n"RESET, fd, g_env.buffer);
	send_data(fd, g_env.buffer, MSG_SIZE);
}
