/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cparse.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfeng <zfeng@student.42.us.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/04 16:38:55 by zfeng             #+#    #+#             */
/*   Updated: 2018/09/05 15:14:51 by sding            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CPARSE_H
# define CPARSE_H

# define CFLAGS nph

# include "client.h"

typedef struct	s_env
{
	char	team_name[MAX_TEAM_NAME];
	char	port[8];
	char	host[16];

}				t_env;

void	client_usage(void);
void	parse_cargs(char **argv);

extern t_env	g_env;

#endif
