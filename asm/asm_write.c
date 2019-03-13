/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asm_write.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/13 13:22:24 by prastoin          #+#    #+#             */
/*   Updated: 2019/03/13 18:25:43 by prastoin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"
#include "ft_string.h"

void		fill_uint(t_read *rd, size_t nb)
{
	ft_puthar_fd(rd->fd2, (nb / 16777216) % 256);
	ft_puthar_fd(rd->fd2, (nb / 65536) % 256);
	ft_puthar_fd(rd->fd2, (nb / 256) % 256);
	ft_puthar_fd(rd->fd2, nb % 256);
}

ssize_t		fill_str(t_read *rd, size_t max_len)
{
	size_t	len;

	rd->index += 1;
	len = 0;
	while (1)
	{
		if (rd->index == rd->len)
			if (io_fill(rd) <= 0)
				ex_error("Incomplete name / Parse error.\n");
		if (rd->buffer[rd->index] == '"')
			break ;
		ft_puthar_fd(rd->fd2, rd->buffer[rd->index]);
		rd->index++;
		len++;
	}
	if (len == 0)
		ex_error("Invalid name / comment.\n");
	while (len < max_len)
	{
		ft_puthar_fd(rd->fd2, '\0');
		len++;
	}
	return (0);
}

ssize_t		header(t_read *rd)
{
	uint8_t str[9];

	fill_uint(rd, COREWAR_EXEC_MAGIC);
	if ((io_read(rd, str, 5) < 0) || (ft_strncmp(str, ".name", 5) != 0))
		ex_error("No \".name\".\n");
	io_moveto(rd, '"');
	fill_str(rd, 132);
	rd->index += 1;
	io_moveto(rd, '\n');
	rd->index += 1;
	fill_uint(rd, 0);
	if ((io_read(rd, str, 8) < 0) || (ft_strncmp(str, ".comment", 8) != 0))
		ex_error("No \".comment\".\n");
	io_moveto(rd, '"');
	fill_str(rd, 2052);
	return (0);
}
