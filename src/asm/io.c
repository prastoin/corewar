/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/04 15:33:01 by prastoin          #+#    #+#             */
/*   Updated: 2019/05/22 22:01:42 by prastoin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_string.h"

void		io_seek(t_write *out, ssize_t offset, bool set_cur)
{
	if (out->flushable)
	{
		io_flush(out);
		lseek(out->fd, offset, (set_cur ? SEEK_SET : SEEK_CUR));
	}
	else
	{
		if (set_cur)
			out->index = offset;
		else
			out->index += offset;
	}
}

void		io_write_read(t_write *out, uint8_t *tmp, size_t size)
{
	if (out->flushable)
	{
		io_flush(out);
		read(out->fd, tmp, size);
	}
	else
	{
		if (out->index < out->buffer_size)
		{
			ft_memcpy(tmp, out->buffer + out->index, size);
			out->index += size;
		}
	}
}
