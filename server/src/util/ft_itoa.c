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

#include <server.h>

static char		*ft_strnew(size_t size)
{
	char *str;

	if (!(str = (char *)malloc(sizeof(char) * (size + 1))))
		return (NULL);
	bzero(str, size + 1);
	return (str);
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