/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mem_io.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/03 15:15:12 by prastoin          #+#    #+#             */
/*   Updated: 2019/04/03 16:53:18 by prastoin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

void		mem_write(char mem[], char *value, size_t offset, size_t size)
{
	ft_memcpy(mem + offset, value, size);
}

size_t		mem_read(char mem[], uint8_t str[], size_t offset, size_t size)
{
	size_t value;
	size_t i;

	ft_memcpy(str, mem + offset, size);
	i = 0;
	value = 0;
	while (i < size)
	{
		value = value * 256 + str[i];
		i++;
	}
	return (value);
}

void		mem_write_one(char mem[], char c, size_t offset)
{
	mem_write(mem, &c, offset, 1);
}

void		mem_write_int(char mem[], size_t nb, size_t len, size_t offset)
{
	while (len != 0)
	{
		len--;
		mem_write_one(mem, (nb >> (len * 8)) & 0xFF, offset);
	}
}
