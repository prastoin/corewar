/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mem_io.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/03 15:15:12 by prastoin          #+#    #+#             */
/*   Updated: 2019/05/07 13:05:56 by fbecerri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

void		mem_read(const uint8_t mem[MEM_SIZE], uint8_t *dst,
		ssize_t offset, size_t n)
{
	ssize_t i;

	while (offset < 0)
		offset += MEM_SIZE;
	i = n - 1;
	while (i >= 0)
	{
		offset = offset % MEM_SIZE;
		(dst)[i] = mem[offset + i];
		i--;
	}
}

void		mem_write(uint8_t mem[MEM_SIZE], const uint8_t *src,
		ssize_t offset, size_t n)
{
	ssize_t i;

	i = n - 1;
	while (offset < 0)
		offset += MEM_SIZE;
	while (i >= 0)
	{
		offset %= MEM_SIZE;
		mem[offset + i] = (src)[i];
		i--;
	}
}

void		mem_write_one(uint8_t mem[MEM_SIZE], uint8_t c,
		ssize_t offset)
{
	mem_write(mem, &c, offset, 1);
}

void		mem_write_int(uint8_t mem[MEM_SIZE], size_t nb, size_t len,
		ssize_t offset)
{
	while (len != 0)
	{
		len--;
		mem_write_one(mem, (nb >> (len * 8)) & 0xFF, offset);
	}
}
