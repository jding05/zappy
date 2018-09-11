/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmds.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfeng <zfeng@student.42.us.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/10 23:06:58 by zfeng             #+#    #+#             */
/*   Updated: 2018/09/10 23:07:00 by zfeng            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cmds.h"

void	cmd_advance(t_reqq	*reqq)
{
	// send_data("ok", MSG_SIZE);
	printf("\n******advance executed******\n");	
}

void	cmd_broadcast(t_reqq	*reqq)
{
	char	*text;

	// text = ft_strsplit(cmd)[1];
	printf("\n******broadcast executed******\n");	
}

void	cmd_connect_nbr(t_reqq	*reqq)
{
	printf("\n******connect_nbr executed******\n");
}

void	cmd_fork(t_reqq	*reqq)
{
	printf("\n******fork executed******\n");
}

void	cmd_hatch(t_reqq	*reqq)
{
	printf("\n******hatch executed******\n");
}

void	cmd_incantation(t_reqq	*reqq)
{
	printf("\n******incantation executed******\n");
}

void	cmd_inventory(t_reqq	*reqq)
{
	printf("\n******inventory executed******\n");
}

void	cmd_kick(t_reqq	*reqq)
{
	printf("\n******kick executed******\n");
}

void	cmd_left(t_reqq	*reqq)
{
	printf("\n******left executed******\n");
}

void	cmd_put(t_reqq	*reqq)
{
	
	printf("\n******put executed******\n");
}

void	cmd_right(t_reqq	*reqq)
{
	printf("\n******right executed******\n");
}

void	cmd_see(t_reqq	*reqq)
{
	printf("\n******see executed******\n");
}

void	cmd_take(t_reqq	*reqq)
{
	
	printf("\n******take executed******\n");
}

t_cmd	g_cmd_table[13] = 
{
	{"advance", 7, cmd_advance},
	{"broadcast", 7, cmd_broadcast},
	{"connect_nbr", 0, cmd_connect_nbr},
	{"fork", 42, cmd_fork},
	{"hatch", 600, cmd_hatch},
	{"incantation", 300, cmd_incantation},
	{"inventory", 1, cmd_inventory},
	{"kick", 7, cmd_kick},
	{"left", 7, cmd_left},
	{"put", 7, cmd_put},
	{"right", 7, cmd_right},
	{"see", 7, cmd_see},
	{"take", 7, cmd_take},
};
