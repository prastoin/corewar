/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/13 10:05:53 by prastoin          #+#    #+#             */
/*   Updated: 2019/03/14 15:57:06 by prastoin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_string.h"


void	ft_puthar_fd(size_t fd, size_t c)
{
	write (fd, &c, 1);
}

ssize_t		ft_strcmp(const char *s1, const char *s2)
{
	size_t			i;
	unsigned char	*str1;
	unsigned char	*str2;

	str1 = (unsigned char *)s1;
	str2 = (unsigned char *)s2;
	i = 0;
	while (str1[i] == str2[i] && str1[i] && str2[i])
		i++;
	return (str1[i] - str2[i]);
}

ssize_t		ft_strncmp(uint8_t *s1, char *str, size_t n)
{
	size_t		i;

	i = 0;
	while (i < n)
	{
		if (str[i] -  s1[i] != 0)
			return (str[i] - s1[i]);
		if (str[i] == '\0' && s1[i] == '\n')
			return (0);
		i++;
	}
	return (0);
}

size_t	ft_strlen(const char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

size_t		search_for(uint8_t *str, uint8_t c)
{
	size_t	i;
	size_t	count;

	count = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == c)
			count++;
		i++;
	}
	return (count);
}

uint8_t		*ft_strcpy(uint8_t *dst, const uint8_t *src)
{
	size_t	i;

	i = 0;
	while (src[i])
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (dst);
}
