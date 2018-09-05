/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfeng <zfeng@student.42.us.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/04 16:38:55 by zfeng             #+#    #+#             */
/*   Updated: 2018/09/04 22:31:08 by zfeng            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_H
# define PARSE_H

# include "server_client.h"

# define SFLAGS pxynct

typedef struct	s_env
{
	char	port[8];
	int		map_x;
	int		map_y;
	int		max_player;
	int		nb_team;
	int		nb_client;
	int		tud;
}				t_env;

void	parse_args(char **argv);
void	server_usage(void);

#endif
