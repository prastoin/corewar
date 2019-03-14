/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/12 14:50:32 by prastoin          #+#    #+#             */
/*   Updated: 2019/03/14 18:03:22 by prastoin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"
#include "ft_string.h"

uint32_t	io_readnum(t_read *rd)
{
	int16_t		c;
	uint32_t	res;
	bool		sign;

	res = 0;
	if ((c = io_peek(rd)) == '-' || c == '+')
	{
		sign = c == '-';
		rd->index++;
	}
	while ((c = io_peek(rd)) >= '0' && c <= '9')
	{
		res = res * 10 + (c - '0');
		rd->index++;
	}
	return (sign ? -res : res);
}

bool		io_skip(t_read *rd, char e)
{
	int16_t c;

	while ((c = io_peek(rd)) != -1 && c != e )
		rd->index++;
	if (c == -1)
		return (false);
	else
	{
		rd->index++;
		return (true);
	}
}

ssize_t		io_fill(t_read *rd)
{
	ssize_t	ret;

	rd->index = 0;
	ret = read(rd->fd, rd->buffer, BUFFER_SIZE);
	return (ret < 0 ? ret : (rd->len = ret));
}

t_read		init_read(int fd)
{
	t_read	rd;

	rd = (t_read){
		.index = 0,
		.len = 0,
		.nbr_read = 0,
		.fd = fd
	};
	return (rd);
}

int16_t		io_peek(t_read *rd)
{
	if (rd->index == rd->len)
		if (io_fill(rd) <= 0)
			return (-1);
	return (rd->buffer[rd->index]);
}

bool		io_expect(t_read *rd, const char *str)
{
	while (*str)
		if (io_peek(rd) == *str)
		{
			rd->index++;
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
	return (data_len + i);
}

/*int		main(int argc, const char *argv[])
{
	t_read		rd;
	size_t		fd;
	uint8_t		test[8];
	size_t		r;

	if ((fd = open(argv[1], O_RDWR)) == -1)
		return (-1);
	rd = init_read(fd);
	while ((r = io_read(&rd, test, 8)) != 0)
	{
		printf("%.*s", r, test);
	}
	return (0);
}*/
