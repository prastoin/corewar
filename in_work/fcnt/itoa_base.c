/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   itoa_base.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/13 14:35:26 by prastoin          #+#    #+#             */
/*   Updated: 2019/03/28 09:39:39 by prastoin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_string.h"

ssize_t		ft_itoa_base(uintmax_t nb, char *str, uint8_t b, const char *base)
{
	uintmax_t	tmp;
	uint8_t		i;
	uint8_t		len;

	i = 1;
	tmp = nb;
	if (b < ft_strlen(base))
		return (-1);
	while (tmp /= b)
		i++;
	len = i;
	str[i] = '\0';
	while (i--)
	{
		str[i] = base[nb % b];
		nb /= b;
	}
	return (len);
}

void		ft_itoa_hexa(char *str, uintmax_t nb, size_t len)
{
	uint8_t i;
	const char *base = "0123456789abcdef";

	str[len + 2] = '\0';
	i = len + 2;
	while (i--)
	{
		str[i] = base[nb % 16];
		nb /= 16;
	}
	str[0] = '0';
	str[1] = 'x';
}
