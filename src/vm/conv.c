/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conv.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbecerri <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/06 01:44:33 by fbecerri          #+#    #+#             */
/*   Updated: 2019/05/22 23:13:09 by prastoin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

intmax_t	conv_bin_num(uint8_t *mem, size_t len)
{
	uintmax_t	num;
	size_t		i;

	i = 0;
	num = 0;
	while (i < len)
	{
		num <<= 8;
		num |= mem[i];
		i++;
	}
	if (mem[0] & 0x80)
	{
		while (i < sizeof(num))
		{
			num |= ((uintmax_t)0xFF << (i * 8));
			i++;
		}
	}
	return (num);
}
