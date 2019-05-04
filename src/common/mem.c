/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/27 16:15:45 by prastoin          #+#    #+#             */
/*   Updated: 2019/05/04 14:22:52 by prastoin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_string.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	size_t	i;

	i = 0;
	if (n == 0)
		return (dst);
	while (i < n)
	{
		((char *)dst)[i] = ((char *)src)[i];
		i++;
	}
	return (dst);
}

void		*ft_memset(void *s, int c, size_t n)
{
	size_t i;

	i = 0;
	while (i < n)
	{
		((char *)s)[i] = c;
		i++;
	}
	return (s);
}

void		*ft_memchr(const void *s, int c, size_t n)
{
	size_t i;

	i = 0;
	if (n != 0)
	{
		while (((char *)s)[i] && i < n)
		{
			if (((char *)s)[i] == (char)c)
				return ((char *)s + i);
			if (((char *)s)[i] == '\0' && (char)c == '\0')
				return ((char *)s + i);
			i++;
		}
	}
	return (NULL);
}
