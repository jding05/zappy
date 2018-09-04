/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sding <sding@student.42.us.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/14 16:50:07 by sding             #+#    #+#             */
/*   Updated: 2018/08/14 17:06:21 by sding            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #ifndef CMD_H
// # define CMD_H
//
// # include "server.h"
// # include "data_structures.h"
//
// typedef struct	s_cmd
// {
// 	char		*cmd;
// 	int			delay_time;
// 	void		(*func)(t_players players, char *msg);
// }				t_cmd;
//
// /*
// ****************************************
// ** this is the dispatch table in cmd.c *
// ****************************************
// * * the sign o -> is the note that the function has been handled
//
// t_cmd	g_cmd[] =
// {
// o	{"advance", 7, cmd_advance},			// advance one square
// o	{"right", 7, cmd_right},				// turn right 90 degrees
// o	{"left", 7, cmd_left},					// turn left 90 degrees
// o	{"see", 7, cmd_see},					// see
// o	{"inventory", 1, cmd_inventory},		// inventory
// o	{"take", 7, cmd_take},					// take an object
// o	{"put", 7, cmd_put},					// put down an object
// o	{"kick", 7, cmd_kick},					// kick a player from the square
// o	{"braodcast", 7, cmd_broadcast},		// broadcast
// o	{"incantation", 300, cmd_incantation},	// begin the incantation
// 	{"fork", 42, cmd_fork},					// fork a player
// o	{"connect_nbr", 0, cmd_connect_nbr},	// know the number of unused connections by the team
//
// 	{"hatch", 600, cmd_hatch}				// after fork (lay the egg), the lag need 600 cycle to hatch
// 	//{"", , cmd_death},// deadth -> clear out the queue and FD_CLR fd_sets;
// 	{NULL, 0, NULL},
// }
// *****************************************
// */
//
// #endif
