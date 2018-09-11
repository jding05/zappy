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

#include "../../inc/server.h"

/*
** cmd_hatch will add to queue after the fork 42 cycle done
*/

const t_cmd		g_cmd[16] =
{
	{"advance", 7, cmd_advance},
	{"right", 7, cmd_right},
	{"left", 7, cmd_left},
	{"see", 7, cmd_see},
	{"inventory", 1, cmd_inventory},
	{"take", 7, cmd_take},
	{"put", 7, cmd_put},
	{"kick", 7, cmd_kick},
	{"broadcast", 7, cmd_broadcast},
	{"incantation", 300, cmd_incantation},
	{"fork", 42, cmd_fork},
	{"connect_nbr", 0, cmd_connect_nbr},
	{"hatch", 600, cmd_hatch},
	{"pos", 0, cmd_pos},
	{"info", 0, cmd_info},
	{NULL, 0, NULL}
};

const int		g_max_res[7] = {0, 54, 60, 60, 30, 36, 6};
const char		*g_res_name[7] =
{"food", "linemate", "deraumere", "sibur", "mendiane", "phiras", "thystame"};

const char		*g_options[6] = {"-p", "-x", "-y", "-n", "-c", "-t"};
