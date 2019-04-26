/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/13 10:05:53 by prastoin          #+#    #+#             */
/*   Updated: 2019/04/26 17:07:55 by prastoin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_string.h"

char		*ft_strchr(const char *s, int c)
{
	size_t	i;
	size_t	max;

	i = 0;
	max = ft_strlen(s);
	while (s[i] != c && s[i])
		i++;
	if (i == max && s[i] != c)
		return (0);
	else
		return ((char *)(s + i));
}

intmax_t	ft_atoi(char *str)
{
	intmax_t	res;
	int			neg;
	size_t		i;

	res = 0;
	i = 0;
	neg = 1;
	while (str[i] == ' ' || (str[i] >= '\a' && str[i] <= '\r'))
		i++;
	if (str[i] == '-')
		neg = -1;
	if (str[i] == '+' || neg == -1)
		i++;
	while (str[i] && (str[i] >= '0' && str[i] <= '9'))
	{
		res = res * 10 + (str[i] - '0');
		i++;
	}
	return (res * neg);
}

size_t	nb_len(uintmax_t n)
{
	size_t	len;

	len = 1;
	while (n /= 10)
		len++;
	return (len);
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
