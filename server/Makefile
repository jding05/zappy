SRV = server
LIB = libft.a
#LIBDEBUG = ""
CFLAGS += -Wall -Werror -Wextra
INCLUDES = -I./inc #-I lib/inc/
#LINK = -L lib/ -l ft
CC = gcc

################################################################################
# Source directories identifiers                                               #
################################################################################

SRCDIR = src/
SRCDIR_SRV = server/
SRCDIR_UTIL = util/
# SRCDIR_BRD = board/
# SRCDIR_CLN = client/
# SRCDIR_COM = communication/
# SRCDIR_DTH = death/
# SRCDIR_EGG = egg/
# SRCDIR_ERR = error/
# SRCDIR_EVS = events/
# SRCDIR_EVS_GFX = graphic/
# SRCDIR_EVS_PLR = player/
# SRCDIR_EVS_SRV = server/
# SRCDIR_GFX = graphic/
# SRCDIR_GFX_TRS = transmit/
# SRCDIR_INV = inventory/
# SRCDIR_PLR = player/
# SRCDIR_SRV = server/
# SRCDIR_TEM = team/
# SRCDIR_TIM = time/

OBJSRC = $(patsubst %, %.o, $(addprefix $(addprefix $(SRCDIR), $(SRCDIR_SRV)), $(SRC_SRV)))
OBJSRC += $(patsubst %, %.o, $(addprefix $(addprefix $(SRCDIR), $(SRCDIR_UTIL)), $(SRC_UTIL)))
# OBJSRC = $(patsubst %, %.o, $(addprefix $(addprefix $(SRCDIR), $(SRCDIR_BRD)), $(SRC_BRD)))
# OBJSRC += $(patsubst %, %.o, $(addprefix $(addprefix $(SRCDIR), $(SRCDIR_CLN)), $(SRC_CLN)))
# OBJSRC += $(patsubst %, %.o, $(addprefix $(addprefix $(SRCDIR), $(SRCDIR_COM)), $(SRC_COM)))
# OBJSRC += $(patsubst %, %.o, $(addprefix $(addprefix $(SRCDIR), $(SRCDIR_DTH)), $(SRC_DTH)))
# OBJSRC += $(patsubst %, %.o, $(addprefix $(addprefix $(SRCDIR), $(SRCDIR_EGG)), $(SRC_EGG)))
# OBJSRC += $(patsubst %, %.o, $(addprefix $(addprefix $(SRCDIR), $(SRCDIR_ERR)), $(SRC_ERR)))
# OBJSRC += $(patsubst %, %.o, $(addprefix $(addprefix $(SRCDIR), $(SRCDIR_EVS)), $(SRC_EVS)))
# OBJSRC += $(patsubst %, %.o, $(addprefix $(addprefix $(SRCDIR), $(addprefix $(SRCDIR_EVS), $(SRCDIR_EVS_GFX))), $(SRC_EVS_GFX)))
# OBJSRC += $(patsubst %, %.o, $(addprefix $(addprefix $(SRCDIR), $(addprefix $(SRCDIR_EVS), $(SRCDIR_EVS_PLR))), $(SRC_EVS_PLR)))
# OBJSRC += $(patsubst %, %.o, $(addprefix $(addprefix $(SRCDIR), $(addprefix $(SRCDIR_EVS), $(SRCDIR_EVS_SRV))), $(SRC_EVS_SRV)))
# OBJSRC += $(patsubst %, %.o, $(addprefix $(addprefix $(SRCDIR), $(SRCDIR_GFX)), $(SRC_GFX)))
# OBJSRC += $(patsubst %, %.o, $(addprefix $(addprefix $(SRCDIR), $(addprefix $(SRCDIR_GFX), $(SRCDIR_GFX_TRS))), $(SRC_GFX_TRS)))
# OBJSRC += $(patsubst %, %.o, $(addprefix $(addprefix $(SRCDIR), $(SRCDIR_INV)), $(SRC_INV)))
# OBJSRC += $(patsubst %, %.o, $(addprefix $(addprefix $(SRCDIR), $(SRCDIR_PLR)), $(SRC_PLR)))
# OBJSRC += $(patsubst %, %.o, $(addprefix $(addprefix $(SRCDIR), $(SRCDIR_SRV)), $(SRC_SRV)))
# OBJSRC += $(patsubst %, %.o, $(addprefix $(addprefix $(SRCDIR), $(SRCDIR_TEM)), $(SRC_TEM)))
# OBJSRC += $(patsubst %, %.o, $(addprefix $(addprefix $(SRCDIR), $(SRCDIR_TIM)), $(SRC_TIM)))

################################################################################
# SOURCE FILES                                                                 #
################################################################################

SRC_SRV = server

SRC_UTIL = read_flags

# SRC_BRD = \
# 	board \
# 	tile

# SRC_CLN = \
# 	client

# SRC_COM = \
# 	communication

# SRC_DTH = \
# 	death

# SRC_EGG = \
# 	death \
# 	egg \
# 	place \
# 	pool

# SRC_ERR = \
# 	error

# SRC_EVS = \
# 	events \
# 	pool \
# 	queue

# SRC_EVS_PLR = \
# 	commandset_1 \
# 	commandset_2 \
# 	see \
# 	incantation \
# 	kick \
# 	broadcast

# SRC_EVS_SRV = \
# 	actions

# SRC_GFX = \
# 	graphic

# SRC_GFX_TRS = \
# 	transmit \
# 	tiles \
# 	teams \
# 	players \
# 	eggs \
# 	incantation

# SRC_INV = \
# 	inventory \
# 	minus1 \
# 	minus2 \
# 	plus1 \
# 	plus2

# SRC_PLR = \
# 	death \
# 	place \
# 	player \
# 	pool

# SRC_SRV = \
# 	io \
# 	main \
# 	srv_sets1 \
# 	srv_sets2

# SRC_TEM = \
# 	team

# SRC_TIM = \
# 	time

################################################################################
# SOURCE FILES                                                                 #
################################################################################

all: $(SRV) #$(LIB) 

debug: $(LIBDEBUG)
debug: $(SRV)

$(SRV): $(OBJSRC)
	$(CC) $(CFLAGS) $(INCLUDES) $(OBJSRC) -o $(SRV) 
#$(LINK) 
	
$(LIB):
	make -C lib/

#$(LIBDEBUG):
#	make debug -C lib/
%.o: %.c
	@ echo "$(YELLOW)Compiling $<...$(RES)"
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# $(OBJSRC): $(OBJSRC:%.o=%.c)
# 	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@
# 	echo $(OBJSRC)

#$(OBJ_DIR)%.o:$(SRC_DIR)%.c
#	gcc -g -Wall -Werror -Wextra -c -I $(LIBFT_DIR)libft.h -I includes  -o $@ -c $<

clean:
	rm -f $(OBJSRC)
#	make clean -C lib/
	
fclean: clean
	rm -f $(SRV)
#	rm -f lib/$(LIB)
	
re: fclean all
	