# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: zfeng <zfeng@student.42.us.org>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/09/14 21:47:05 by zfeng             #+#    #+#              #
#    Updated: 2018/09/14 21:47:08 by zfeng            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# targets
CLI := client

# compiler and flags
CC = gcc
CFLAG = -c
WFLAG = -Wall -Wextra -Werror

# headers
HEADER = -I include

# files and directories
SRC =	parse_args.c \
		select_client.c \
		send_recv.c \
		validate_req.c

OBJ =	$(SRC:.c=.o)
SRCDIR = src/
OBJDIR = objs/
SRCS = $(addprefix $(SRCDIR), $(SRC))
OBJS = $(addprefix $(OBJDIR), $(OBJ))

.PHONY: all clean fclean re
.SUFFIXES: .c .o

all:	$(CLI)

# COMPILING CLIENT
$(OBJDIR)%.o: $(SRCDIR)%.c
	@/bin/mkdir -p $(OBJDIR)
	@$(CC) $(CFLAG) $(WFLAG) $(HEADER) $< -o $@
	# @$(CC) $(CFLAG) $(HEADER) $< -o $@

$(CLI): $(OBJS)
	@$(CC) $(OBJS) -o $@
	@echo "\033[32mplayer client compiled.\033[0m"

clean:
	@/bin/rm -rf objs
	@echo "\033[32mobject files removed.\033[0m"

fclean: clean
	@/bin/rm -rf $(CLI) $(LIBFT)libft.a
	@echo "\033[32mexecutables removed.\033[0m"

re: fclean all

