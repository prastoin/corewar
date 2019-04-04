/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mem_io.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/03 15:15:12 by prastoin          #+#    #+#             */
/*   Updated: 2019/04/04 14:37:14 by prastoin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

void		mem_write(char mem[], uint8_t *value, size_t offset, size_t size)
{
	ft_memcpy(mem + offset, value, size);
}

void	mem_read(char mem[], uint8_t str[], size_t offset, size_t size)
{
	ft_memcpy(str, mem + offset, size);
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
