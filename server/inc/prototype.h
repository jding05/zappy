/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prototype.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sding <sding@student.42.us.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/03 14:29:20 by sding             #+#    #+#             */
/*   Updated: 2018/09/03 14:29:21 by sding            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PROTOTYPE_H
# define PROTOTYPE_H

# include "server.h"
/*
** server.c
*/

void	server_usage(void);
void	game_loop(void);
void	free_malloc(void);
void	check_dead_player(void);
void	server_client_connection(void);
int		check_winner(void);
void	generate_resource(void);

/*
** read_flags.c
*/

void	print_flags(t_env *env);
void	update_max_player_per_team(void);
void	print_team(void);
int		read_flags(int argc, char **argv, t_env *env);
int		team_init(char **argv, int i, int nb_team);
int		set_value(char **flag, int i, int m, t_env *env);
int		isnbr_str(char *str);

/*
** exec_event_queue.c
*/

int		check_event_time(struct timeval *curr_time, struct timeval *exec_time);
void	exec_event_list(int st_term);
void	exec_event(t_event **event, t_event **prev, t_event **h, t_event **l);
void	exec_event_queue(int short_term);

/*
** cmd [folder]
*/

/*
** advance
*/

int		cmd_advance(t_players players, char *msg);

/*
** broadcast
*/

int		cmd_broadcast(t_players players, char *msg);
void	cal_four_pos(int pos[4][2], int y, int x);
int		get_closest_pos(int pos[4][2], int pos_y, int pos_x);
int		calc_direction(int pos[2], int y, int x, int direction);
void	send_braodcast_msg(int nb_dir, int fd, char *msg);
void	broadcast(int y, int x, int fd, char *msg);

/*
** connect_nbr
*/

int		cmd_connect_nbr(t_players players, char *msg);

/*
** fork
*/

int     cmd_fork(t_players players, char *msg);
void	calc_time_spead(void); /////////////////////// -> this should be in the parsing
void	init_live(int fd);
void	update_live(int fd, int food);
void	record_time(t_event *node, int delay_time);
t_event	*init_event_node(int fd, char *msg, int delay_time);
void    push_cmd_hatch(int fd, char *msg);
void    laid_egg(t_players *players);

/*
** hatch
*/

int		cmd_hatch(t_players players, char *msg);

/*
** incantation
*/

int		cmd_incantation(t_players players, char *msg);
void	level_up_and_unblock(int count, int fds[100]);
int		check_prerequest(int level, int i);
int		level_require(int level);
int		cmd_incantation_check(t_players players, char *msg);
void	blocking(int count, int fds[100]);

/*
** inventory
*/

int		cmd_inventory(t_players players, char *msg);

/*
** kcik
*/

int		cmd_kick(t_players players, char *msg);
void	send_kick_msg(t_players players, int direction);
void	update_player_pos(t_players players);

/*
** left
*/

int		cmd_left(t_players players, char *msg);

/*
** put
*/

int		cmd_put(t_players players, char *msg);
int		send_msg(int fd, char *status, char *error_msg);
/*
** right
*/

int		cmd_right(t_players players, char *msg);

/*
** see
*/

void	print_player_info(t_players players);
void	find_cell_player(int y, int x);
void	print_resource(int y, int x);
void	print_cell_value(int y, int x);
void	see_north_area(int level, int y, int x);
void	see_south_area(int level, int y, int x);
void	see_east_area(int level, int y, int x);
void	see_west_area(int level, int y, int x);
int		cmd_see(t_players players, char *msg);

/*
** take
*/

int     check_resource(char *msg);
int     cmd_take(t_players players, char *msg);

#endif
