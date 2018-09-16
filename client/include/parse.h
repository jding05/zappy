/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfeng <zfeng@student.42.us.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/04 16:38:55 by zfeng             #+#    #+#             */
/*   Updated: 2018/09/10 23:09:27 by zfeng            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_H
# define PARSE_H

# define CFLAGS nph

# include "client.h"

# define MAX_TEAM_NAME 32

typedef struct	s_env
{
	char		team_name[MAX_TEAM_NAME];
	char		port[8];
	char		host[16];

}				t_env;

void			client_usage(void);
int				parse_args(char **av);

extern t_env	g_env;

#endif
