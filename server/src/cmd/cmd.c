/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sding <sding@student.42.us.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/14 16:22:15 by sding             #+#    #+#             */
/*   Updated: 2018/08/14 16:49:17 by sding            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cmd.h"

t_cmd	g_cmd[] =
{
	{"advance", 7, cmd_advance},
	{"right", 7, cmd_right},
	{"left", 7, cmd_left},
	{"see", 7, cmd_see},
	{"inventory", 1, cmd_inventory},
	{"take", 7, cmd_take},
	{"put", 7, cmd_put},
	{"kick", 7, cmd_kick},
	{"braodcast", 7, cmd_broadcast},
	{"incantation", 300, cmd_incantation},
	{"fork", 42, cmd_fork},
	{"connect_nbr", 0, cmd_connect_nbr},
	{"hatch", 600, cmd_hatch}, // this cmd will add to queue after the fork 42 cycle done
	//{"", 7, cmd_advance},// deadth -> clear out the queue and FD_CLR fd_sets;
	{NULL, 0, NULL},
}
