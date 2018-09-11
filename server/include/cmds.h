/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmds.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfeng <zfeng@student.42.us.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/10 23:18:04 by zfeng             #+#    #+#             */
/*   Updated: 2018/09/10 23:18:05 by zfeng            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMDS_H
# define CMDS_H
# include "server.h"

# define REQ_SIZE 64

typedef struct	s_cmd
{
	char	*cmd;
	int		delay_time;
	void	(*func)();
}				t_cmd;

void	cmd_advance(t_reqq	*reqq);
void	cmd_broadcast(t_reqq	*reqq);
void	cmd_connect_nbr(t_reqq	*reqq);
void	cmd_fork(t_reqq	*reqq);
void	cmd_hatch(t_reqq	*reqq);
void	cmd_incantation(t_reqq	*reqq);
void	cmd_inventory(t_reqq	*reqq);
void	cmd_kick(t_reqq	*reqq);
void	cmd_left(t_reqq	*reqq);
void	cmd_put(t_reqq	*reqq);
void	cmd_right(t_reqq	*reqq);
void	cmd_see(t_reqq	*reqq);
void	cmd_take(t_reqq	*reqq);

extern t_cmd	g_cmd_table[13];

#endif
