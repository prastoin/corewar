/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io_write.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/04 15:09:06 by prastoin          #+#    #+#             */
/*   Updated: 2019/06/04 18:37:54 by fbecerri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"
#include "ft_string.h"

void	io_flush(t_write *out)
{
	if (out->flushable == true)
	{
		if (out->index)
			write(out->fd, out->buffer, out->index);
	}
	else
		out->fd = -1;
	out->index = 0;
}

void	io_write(t_write *out, void *o_data, size_t size)
{
	size_t	space;
	uint8_t	*data;

	data = o_data;
	space = out->buffer_size - out->index;
	while (space < size)
	{
		ft_memcpy(out->buffer + out->index, data, space);
		out->index += space;
		io_flush(out);
		data += space;
		size -= space;
		out->nbr_write += space;
		space = out->buffer_size;
	}
	ft_memcpy(out->buffer + out->index, data, size);
	out->index += size;
	out->nbr_write += size;
}

void	io_pad(t_write *out, char c, ssize_t value)
{
	ssize_t	i;

	i = 0;
	while (i < value)
	{
		io_write(out, &c, 1);
		i++;
	}
}
