#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_X 20
#define MAX_Y 20
#define BUF_SIZE 5599

// BUF_SIZE = MAX_X * MAX_Y * 7 * 2 - 1

int		main(int ac, char **av)
{
	int		x = atoi(av[1]);
	int		y = atoi(av[2]);
	int		i = 0;
	int		j;
	int		r;

	printf("%d,%d,", x, y);
	srand(time(NULL));
	while (i < x * y * 7)
	{
		if ((r = rand() % 10) < 10)
		{
			printf("%d,", r);
			i++;
		}
		else
			continue;
	}
	while (i < MAX_X * MAX_Y * 7 - 1)
	{
		printf("#,");
		i++;
	}
	printf("#");
	return (0);
}
