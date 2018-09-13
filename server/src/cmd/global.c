/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   global.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sding <sding@student.42.us.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/14 16:22:15 by sding             #+#    #+#             */
/*   Updated: 2018/09/11 15:22:38 by sding            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/server.h"

/*
** cmd_hatch will add to queue after the fork 42 cycle done
*/
// you don't need specify 16 of the size, when you declare this global, the compiler
// will count how many lines in your array, to allocate the size for you.
// so when you test it, it will iterate thru the whole array,
// bc you set the NULL terminater, so it will stop either when it matched or hit the NULL.
// so in both cases, you don't need to write 16 for it

// when you write the actual array size between the brackets in the array declaration
// -> as a reminder to not exceed the array size

/*
** this is combination of declare + define the array
** when you declare the global or static array,
** C compile will ignore any text between the bracket
** it will count how big your array to give you the memory
*/

const t_cmd		g_cmd[] =
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
	{NULL, 0, NULL}
};

const int		g_max_res[] = {0, 54, 60, 60, 30, 36, 6};
const char		*g_res_name[] =
{"food", "linemate", "deraumere", "sibur", "mendiane", "phiras", "thystame"};

const char		*g_options[6] = {"-p", "-x", "-y", "-n", "-c", "-t"};
