/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/12 14:50:32 by prastoin          #+#    #+#             */
/*   Updated: 2019/05/04 15:24:12 by prastoin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"
#include "ft_string.h"

ssize_t	io_fill(t_read *rd)
{
	ssize_t	ret;

	rd->index = 0;
	ret = read(rd->fd, rd->buffer, BUFFER_SIZE);
	return (ret < 0 ? ret : (rd->len = ret));
}

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

t_read	init_read(int fd, char *argv, bool werror)
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
		.fd = fd,
		.write_able = true,
		.werror = werror
	};
	return (rd);
}

t_write	init_write(void)
{
	t_write		out;

	out = (t_write){
		.index = 0,
		.flushable = false,
		.nbr_write = 0,
	};
	return (out);
}
