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


typedef struct		s_queue
{
	t_event			*first;
	t_event			*last;
}					t_queue

typedef struct		s_event
{
	int				client_fd;
	char			*msg;
	struct timeval	*exec_time;
	struct s_event	*next;
}					t_event;


#endif
