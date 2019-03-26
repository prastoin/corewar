/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/12 14:50:32 by prastoin          #+#    #+#             */
/*   Updated: 2019/03/26 10:48:20 by prastoin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"
#include "ft_string.h"

int32_t	io_readnum(t_read *rd)
{
	int16_t		c;
	uint32_t	res;
	bool		sign;

	res = 0;
	if ((c = io_peek(rd)) == '-' || c == '+')
	{
		sign = c == '-';
		io_next(rd);
	}
	while ((c = io_peek(rd)) >= '0' && c <= '9')
	{
		res = res * 10 + (c - '0');
		io_next(rd);
	}
	return (sign ? -res : res);
}

bool		io_skip(t_read *rd, char e)
{
	int16_t c;

	while ((c = io_peek(rd)) != -1 && c != e )
		io_next(rd);
	if (c == -1)
		return (false);
	else
	{
		io_next(rd);
		return (true);
	}
}

bool		io_skip_until(t_read *rd, char *chars)
{
	int16_t c;
	size_t	i;

	while ((c = io_peek(rd)) != -1)
	{
		i = 0;
		while (chars[i])
		{
			if (chars[i] == c)
				return (true);
			i++;
		}
		io_next(rd);
	}
	return (false);
}

ssize_t		io_fill(t_read *rd)
{
	ssize_t	ret;

	rd->index = 0;
	ret = read(rd->fd, rd->buffer, BUFFER_SIZE);
	return (ret < 0 ? ret : (rd->len = ret));
}

t_read		init_read(int fd, char *argv)
{
	t_read	rd;

	rd = (t_read){
		.index = 0,
		.len = 0,
		.span = (t_span){
				.lines = 1,
				.col = 1,
				.offset = 0,
				.file_name = argv
		},
		.nbr_read = 0,
		.fd = fd
	};
	return (rd);
}

t_write		init_write(int fd)
{
	t_write		out;
	header_t	tmp;

	out = (t_write){
		.index = 0,
		.fd = fd,
		.nbr_write = 0
	};
	return (out);
}

int16_t		io_peek(t_read *rd)
{
	if (rd->index == rd->len)
		if (io_fill(rd) <= 0)
			return (-1);
	return (rd->buffer[rd->index]);
}

void		io_next(t_read *rd)
{
	rd->span.col++;
	rd->span.offset++;
	if (io_peek(rd) == '\n')
	{
		rd->span.col = 1;
		rd->span.lines++;
	}
	rd->index++;
}

bool		io_expect(t_read *rd, const char *str)
{
	while (*str)
		if (io_peek(rd) == *str)
		{
			io_next(rd);
			str++;
		}
		else
			return (false);
	return (true);
}

ssize_t		io_read(t_read *rd, uint8_t data[], size_t data_len)
{
	size_t	remaining;
	size_t	i;
	ssize_t	ret;
	size_t save_data_len;

	remaining = rd->len - rd->index;
	i = 0;
	while (remaining < data_len)
	{
		i += remaining;
		ft_memcpy(data, rd->buffer + rd->index, remaining);
		if ((ret = io_fill(rd)) <= 0)
			return (ret < 0 ? ret : i);
		rd->nbr_read += 1;
		data += remaining;
		data_len -= remaining;
		remaining = rd->len - rd->index;
	}
	ft_memcpy(data, rd->buffer + rd->index, data_len);
	rd->index += data_len;
	data -= save_data_len - data_len;
	i = 0;
	while (i < save_data_len)
	{
		rd->span.col++;
		rd->span.offset++;
		if (data[i] == '\n')
		{
			rd->span.col = 0;
			rd->span.lines++;
		}
			data++;
	}
	return (data_len + i);
}
