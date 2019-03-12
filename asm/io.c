/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/12 14:50:32 by prastoin          #+#    #+#             */
/*   Updated: 2019/03/12 16:48:10 by prastoin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"

/*uint8_t		io_peek(t_read *rd)
{
	if (rd->index == r->len)
}

size_t		io_expect(t_read *rd, const char *wanted)
{
	while (*expected)
		if ()
}*/

ssize_t		io_fill(t_read *rd)
{
	ssize_t	ret;

	rd->index = 0;
	if ((ret = read(rd->fd, rd->buffer, BUFFER_SIZE)) < 0)
		return (ret);
	return (rd->len = ret);
}

t_read		init_read(size_t fd)
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

int		main(int argc, const char *argv[])
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
}
