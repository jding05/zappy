/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfeng <zfeng@student.42.us.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/17 17:05:25 by zfeng             #+#    #+#             */
/*   Updated: 2018/09/17 17:05:28 by zfeng            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/server.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*new;

	if (s1 && s2)
	{
		new = ft_strnew(strlen(s1) + strlen(s2));
		if (!new)
			return (NULL);
		strcpy(new, s1);
		strcat(new, s2);
		return (new);
	}
	return (NULL);
}
