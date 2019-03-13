/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asm_write.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/13 13:22:24 by prastoin          #+#    #+#             */
/*   Updated: 2019/03/13 17:18:41 by prastoin         ###   ########.fr       */
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

ssize_t		fill_name(t_read *rd)
{
	size_t	len;

	len = 0;
	while (rd->buffer[rd->index] != '"')
	{
		ft_puthar_fd(rd->fd2, rd->buffer[rd->index]);
		rd->index++;
		len++;
	}
	while (len < 132)
	{
		ft_puthar_fd(rd->fd2, '\0');
		len++;
	}
	return (0);
}

ssize_t		header(t_read *rd)
{
	uint8_t str[8];

	fill_uint(rd, COREWAR_EXEC_MAGIC);
	if ((io_read(rd, str, 5) < 0) || (ft_strncmp(str, ".name", 5) != 0))
		return (-1);
	if (io_moveto(rd, '"') < 0)
		return (-1);
	fill_name(rd);
	return (0);
}
