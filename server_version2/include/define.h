/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   define.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sding <sding@student.42.us.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/17 20:02:29 by sding             #+#    #+#             */
/*   Updated: 2018/08/17 20:02:31 by sding            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEFINE_H
# define DEFINE_H

/*
** for direction 0 - 3
*/
# define NORTH 0
# define EAST 1
# define SOUTH 2
# define WEST 3

/*
** for colorize ANSI color in printf
*/

# define BLACK		"\033[0;30m"
# define BLUE		"\033[0;34m"
# define CYAN		"\033[1;36m"
# define DARKYELLOW	"\033[0;33m"
# define GREEN		"\033[0;32m"
# define GREY		"\033[1;30m"
# define LAVENDER	"\033[1;35m"
# define LIGHTBLUE	"\033[0;36m"
# define LIGHTGREY	"\033[0;37m"
# define LIME		"\033[1;32m"
# define MAGENTA	"\033[0;35m"
# define PINK		"\033[1;31m"
# define PURPLE		"\033[1;34m"
# define RED		"\033[0;31m"
# define BRED		"\033[1;31m"
# define UNDERLINE	"\033[4m"
# define WHITE		"\033[1;37m"
# define YELLOW		"\033[1;33m"
# define RESET		"\033[0m"

/*
** map x, y value
*/
# define MAX_X 20
# define MAX_Y 10
# define EGG_MAX 6

/*
** socket.c
*/

# define WELCOME "WELCOME 🙂"
# define TEAM_FULL "TEAM IS FULL\nBYE 😕"
# define TEAM_NOT_FOUND "TEAM NAME NOT FOUND\nBYE 😕"
# define TEAM_JOINED "JOINED TEAM"

# define MAX_TEAM 6
# define MAX_FD 77
# define MAP_SIZE 906
# define PLAYER_SIZE 41
# define MSG_SIZE 8192
# define MAX_TIME_UNIT 100
# define MAX_TEAM_NAME 32
# define PAD_CHAR '#'

# define ERROR(s) {perror(s); return(EXIT_FAILURE);}
# define SOCKET_VARS struct addrinfo hints, *ai; struct protoent *proto;
# define SELECT_VARS fd_set master, read_fds; int fdmax;
# define ADD_TEAM_VARS 	int i; int egg_id; char *msg;

#endif
