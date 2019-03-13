/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   itoa_base.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/13 14:35:26 by prastoin          #+#    #+#             */
/*   Updated: 2019/03/13 15:00:43 by prastoin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_string.h"

void	ft_itoa_base(uintmax_t nb, char *str, uint8_t b, const char *base)
{
	uintmax_t	tmp;
	uint8_t		i;

	i = 1;
	tmp = nb;
	if (b < ft_strlen(base))
		return ;
	while (tmp /= b)
		i++;
	str[i] = '\0';
	while (i--)
	{
		str[i] = base[nb % b];
		nb /= b;
	}
	return ;
}
