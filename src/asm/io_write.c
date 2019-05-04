/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io_write.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/04 15:09:06 by prastoin          #+#    #+#             */
/*   Updated: 2019/05/04 15:31:37 by prastoin         ###   ########.fr       */
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
	{
		out->fd = -1;
	}
	out->index = 0;
}

void	io_write_number(t_write *w, uintmax_t n)
{
	uintmax_t	tmp;
	uintmax_t	pow;
	char		c;

	tmp = n;
	pow = 1;
	while (tmp /= 10)
		pow *= 10;
	while (pow)
	{
		c = n / pow + '0';
		io_write(w, &c, 1);
		n %= pow;
		pow /= 10;
	}
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

void	io_write_one(t_write *out, char c)
{
	io_write(out, &c, 1);
}

void	io_write_int(t_write *out, uintmax_t nb, size_t nb_bytes)
{
	while (nb_bytes != 0)
	{
		nb_bytes--;
		io_write_one(out, (nb >> (nb_bytes * 8)) & 0xFF);
	}
}
