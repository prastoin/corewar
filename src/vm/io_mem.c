/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mem_io.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/03 15:15:12 by prastoin          #+#    #+#             */
/*   Updated: 2019/04/19 17:52:20 by prastoin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

void		mem_read(const char mem[], uint8_t *dst, ssize_t offset, size_t n)
{
	size_t i;

	while (offset < 0)
		offset += MEM_SIZE;
	i = 0;
	while (i < n)
	{
		offset = offset % MEM_SIZE;
		(dst)[i] = mem[offset];
		i++;
		offset++;
	}
}

void		mem_write(char mem[], const uint8_t *src, ssize_t offset, size_t n)
{
	size_t i;

	i = 0;
	while (offset < 0)
		offset += MEM_SIZE;
	while (i < n)
	{
		offset %= MEM_SIZE;
		mem[offset] = (src)[i];
		i++;
		offset++;
	}
}

void		mem_write_one(char mem[], uint8_t c, ssize_t offset)
{
	mem_write(mem, &c, offset, 1);
}

void		mem_write_int(char mem[], size_t nb, size_t len, ssize_t offset)
{
	while (len != 0)
	{
		len--;
		mem_write_one(mem, (nb >> (len * 8)) & 0xFF, offset);
	}
}
