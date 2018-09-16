SRV = server
INCLUDES = -I./include
CFLAGS += -Wall -Werror -Wextra -g -fsanitize=address
INCLUDES = -I./inc
CC = gcc

################################################################################
# Source directories identifiers                                               #
################################################################################

SRCDIR = src/
SRCDIR_MAIN = main/
SRCDIR_UTIL = util/
SRCDIR_CMD = cmd/
SRCDIR_EVENT = event/
SRCDIR_SOCKET = socket/
SRCDIR_GLOBAL = global/
SRCDIR_TIME = time/
SRCDIR_RESOURCE = resource/
SRCDIR_CHECK = check/

OBJSRC = $(patsubst %, %.o, $(addprefix $(addprefix $(SRCDIR), $(SRCDIR_MAIN)), $(SRC_MAIN)))
OBJSRC += $(patsubst %, %.o, $(addprefix $(addprefix $(SRCDIR), $(SRCDIR_UTIL)), $(SRC_UTIL)))
OBJSRC += $(patsubst %, %.o, $(addprefix $(addprefix $(SRCDIR), $(SRCDIR_CMD)), $(SRC_CMD)))
OBJSRC += $(patsubst %, %.o, $(addprefix $(addprefix $(SRCDIR), $(SRCDIR_EVENT)), $(SRC_EVENT)))
OBJSRC += $(patsubst %, %.o, $(addprefix $(addprefix $(SRCDIR), $(SRCDIR_SOCKET)), $(SRC_SOCKET)))
OBJSRC += $(patsubst %, %.o, $(addprefix $(addprefix $(SRCDIR), $(SRCDIR_GLOBAL)), $(SRC_GLOBAL)))
OBJSRC += $(patsubst %, %.o, $(addprefix $(addprefix $(SRCDIR), $(SRCDIR_TIME)), $(SRC_TIME)))
OBJSRC += $(patsubst %, %.o, $(addprefix $(addprefix $(SRCDIR), $(SRCDIR_RESOURCE)), $(SRC_RESOURCE)))
OBJSRC += $(patsubst %, %.o, $(addprefix $(addprefix $(SRCDIR), $(SRCDIR_CHECK)), $(SRC_CHECK)))

################################################################################
# SOURCE FILES                                                                 #
################################################################################

SRC_MAIN =	main

SRC_UTIL =	read_flags ft_itoa print_info

SRC_CMD =	cmd_advance cmd_broadcast cmd_connect_nbr cmd_fork cmd_hatch \
			cmd_incantation cmd_incantation_util cmd_inventory cmd_kick \
			cmd_left cmd_put cmd_right cmd_see cmd_see_util cmd_take

SRC_EVENT =	exec_event_queue

SRC_SOCKET = socket send_recv s_utils

SRC_GLOBAL = global

SRC_TIME = time

SRC_RESOURCE = generate_resource

SRC_CHECK = check_dead_player check_winner

################################################################################
# SOURCE FILES                                                                 #
################################################################################

all: $(SRV)

debug: $(LIBDEBUG)
debug: $(SRV)

$(SRV): $(OBJSRC)
	@$(CC) $(CFLAGS) $(INCLUDES) $(OBJSRC) -o $(SRV)

%.o: %.c
	@ echo "$(YELLOW)Compiling $<...$(RES)"
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@rm -f $(OBJSRC)

fclean: clean
	@rm -f $(SRV)

re: fclean all
