#ifndef UTIL_H
# define UTIL_H

# include <stdio.h>
# include <string.h>
# include "data_structures.h"
# include <ctype.h>
# include <fcntl.h>
# include <stdlib.h>

# define DEFAULT_MAP_SIZE 10
# define ARG_FLAGS "pxynct"
# define ARG_CNT 6

int     read_flags(int argc, char **argv, t_env *env);
void    server_usage();


#endif