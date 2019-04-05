/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mem_io.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/03 15:15:12 by prastoin          #+#    #+#             */
/*   Updated: 2019/04/05 16:37:08 by prastoin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

void		mem_read(const char mem[], uint8_t *dst, size_t offset, size_t n)
{
	size_t i;

	i = 0;
	while (i < n)
	{
		offset = offset % MEM_SIZE;
		((char *)dst)[offset] = mem[i];
		i++;
		offset++;
	}
}

void		mem_write(char mem[], const uint8_t *src, size_t offset, size_t n)
{
	size_t i;

	i = 0;
	while (i < n)
	{
		offset = offset % MEM_SIZE;
		mem[i] = ((char *)src)[offset];
		i++;
		offset++;
	}
}

void		mem_write_one(char mem[], uint8_t c, size_t offset)
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
