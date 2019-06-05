/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putf_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/26 15:39:21 by prastoin          #+#    #+#             */
/*   Updated: 2019/06/05 10:40:31 by prastoin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_string.h"

int	ft_putf_fd(int fd, char *fmt, ...)
{
	va_list	args;
	size_t	padd;
	t_write	write;
	uint8_t	buffer[BUFFER_SIZE];

	write = (t_write) {
		.buffer = buffer,
		.buffer_size = sizeof(buffer),
		.flushable = true,
		.fd = fd
	};
	padd = 0;
	va_start(args, fmt);
	ft_putf_va(&write, fmt, args, padd);
	io_flush(&write);
	va_end(args);
	return (0);
}

int	io_putf(t_write *out, char *fmt, ...)
{
	va_list	args;
	size_t	padd;

	padd = 0;
	va_start(args, fmt);
	ft_putf_va(out, fmt, args, padd);
	va_end(args);
	return (0);
}
