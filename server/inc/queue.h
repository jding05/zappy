/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sding <sding@student.42.us.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/17 18:14:45 by sding             #+#    #+#             */
/*   Updated: 2018/08/17 18:14:47 by sding            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef QUEUE_H
# define QUEUE_H

# include "server.h"

# define MAX_MSG 256
# define CMD_LEN 12

typedef struct		s_event
{
	int							fd;
	char						cmd[CMD_LEN];
	char						msg[MAX_MSG];
	struct timeval	*exec_time;
	struct s_event	*next;
}									t_event;

typedef struct		s_st_queue
{
	t_event					*first;
	t_event					*last;
}									t_st_queue;

typedef struct		s_lt_queue
{
	t_event					*first;
	t_event					*last;
}									t_lt_queue;

#endif
