/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sding <sding@student.42.us.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/28 21:46:59 by sding             #+#    #+#             */
/*   Updated: 2018/08/28 21:47:01 by sding            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/server.h"

char		*ft_strnew(size_t size)
{
	char	*new;

	if (!(new = (char*)malloc(sizeof(char) * (size + 1))))
		return (NULL);
	memset(new, 0, size + 1);
	return (new);
}

static int	str_size(int n)
{
	int	i;

	i = 1;
	while (n > 9)
	{
		n /= 10;
		i++;
	}
	return (i);
}

char		*ft_itoa(int n)
{
	char	*str;
	int		size;
	int		neg;

	neg = n < 0 ? 1 : 0;
	if (n == -2147483648)
		return (strdup("-2147483648"));
	n = abs(n);
	size = neg + str_size(n);
	str = ft_strnew(size);
	if (!str)
		return (NULL);
	while (size > 0)
	{
		size--;
		str[size] = abs(n % 10) + '0';
		n /= 10;
	}
	if (neg)
		str[0] = '-';
	return (str);
}

void		num_to_str(int num, char **str, char *delim)
{
	char	*rv;

	rv = ft_itoa(num);
	strcat(*str, rv);
	strcat(*str, delim);
	free(rv);
}
