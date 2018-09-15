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
void	init_res(void);

/*
** read_flags.c
*/

void	print_flags(void);
void	update_max_player_per_team(void);
void	print_team(void);
int		read_flags(int argc, char **argv, t_env *env);
int		team_init(char **argv, int i, int nb_team);
int		set_value(char **flag, int i, int m, t_env *env);
int		isnbr_str(char *str);
void	calc_time_spead(void);
/*
** ft_itoa.c
*/
char	*ft_itoa(int n);
/*
** exec_event_queue.c
*/

int		check_event_time(struct timeval *curr_time, struct timeval *exec_time);
void	cycle_exec_event_loop(void);
//void	exec_event_list(void);
// void	exec_event(t_event **event, t_event **prev, t_event **h, t_event **l);
void	exec_event(void);
t_event	*init_event_node(int fd, char *msg, int delay_time, char *cmd);
void	enqueue(int fd, char *msg);
int		check_valid_cmd(char *msg, char *msg_buf, int i);
// void    long_short_term(t_event *node, int short_term);
void    init_queue(void);
void	print_queue(void);
void	record_time(t_event *node, int delay_time);
int		print_time(struct timeval *now);
// void	exec_event_queue(int short_term);
void	set_block_time(t_event *node, int fd);
// void	dequeue(void);
void	insert(t_event *node);
void	exec_event_and_delete(t_event **event, t_event **prev);

/*
** cmd [folder]
*/

/*
** advance
*/

int		cmd_advance(int fd, char *msg);

/*
** broadcast
*/

int		cmd_broadcast(int fd, char *msg);
void	calc_four_pos(int pos[4][2], int y, int x);  //-----------> Gaetan: fking ugly, Kwame: wtf is this shit
int		get_closest_pos(int pos[4][2], int pos_y, int pos_x);
int		calc_direction(int pos[2], int y, int x, int direction);
void	send_braodcast_msg(int nb_dir, int fd, char *msg);
void	broadcast(int y, int x, int fd, char *msg);

/*
** connect_nbr
*/

int		cmd_connect_nbr(int fd, char *msg);

/*
** fork
*/

int     cmd_fork(int fd, char *msg);
void	update_live(int fd, int food);
void    push_cmd_hatch(int fd);
void    laid_egg(int fd);

/*
** hatch
*/

int		cmd_hatch(int fd, char *msg);

/*
** incantation
*/

int		cmd_incantation(int fd, char *msg);
void	level_up_and_unblock(int count, int fds[MAX_FD]);
int		check_prerequest(int level, int i);
int		level_require(int level);
int		cmd_incantation_check(t_event *node);
void	blocking(int count, int fds[MAX_FD], t_event *node);

/*
** inventory
*/

int		cmd_inventory(int fd, char *msg);

/*
** kick
*/

int		cmd_kick(int fd, char *msg);
void	send_kick_msg(int fd, int direction);
void	update_player_pos(int fd);

/*
** left
*/

int		cmd_left(int fd, char *msg);

/*
** put
*/

int		cmd_put(int fd, char *msg);
// int		send_msg(int fd, char *status, char *error_msg);


/*
** right
*/

int		cmd_right(int fd, char *msg);

/*
** see
*/

void	print_player_info(t_players players);
void	find_cell_player(int y, int x, int flag);
void	print_resource(int y, int x);
void	print_cell_value(int y, int x);
void	see_north_area(int level, int y, int x);
void	see_south_area(int level, int y, int x);
void	see_east_area(int level, int y, int x);
void	see_west_area(int level, int y, int x);
int		cmd_see(int fd, char *msg);

/*
** take
*/

int     check_resource(char *msg);
int     cmd_take(int fd, char *msg);

/*
** socket.c
*/

int		setup_socket(void);
void	s_select_cycles(fd_set *master, fd_set *read_fds, int *fdmax, int lfd);
void	s_select_recv(int fd, fd_set *master);
void	s_select_accept(int fd, fd_set *master, int *fdmax);
int		s_create_socket(char *port, int reuse);
int		s_iter_sock(struct addrinfo *ai, struct protoent *proto, int reuse);
void	*get_in_addr(struct sockaddr *sa);

/*
** send_recv
*/
int		send_data(int fd, char *msg, int ebytes);
char	*recv_data(int fd, int ebytes);

/*
** s_utils
*/
int		perror_rv(char *errmsg);
void	s_init_new_player(int fd);
void	s_init_egg_player(int fd, int team_id, int egg_id);
int	    s_add_to_team(char *team_name, int fd);
void	init_live(int fd);
#endif
