/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reqq.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfeng <zfeng@student.42.us.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/31 15:37:38 by zfeng             #+#    #+#             */
/*   Updated: 2018/09/10 23:18:14 by zfeng            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQQ_H
# define REQQ_H

# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <stdio.h>

# define REQ_SIZE 64
# define CMD_SIZE 16
# define PARAM_SIZE 32

typedef struct		s_reqq
{
	int				fd;
	char			cmd[CMD_SIZE];
	char			param[PARAM_SIZE];
	struct s_reqq	*next;
}					t_reqq;

void	enqueue(t_reqq **head, int fd, char *req);
void	dequeue(t_reqq **head);

void	print_queue(t_reqq *head);
#endif
