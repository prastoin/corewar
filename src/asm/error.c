/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/13 17:54:07 by prastoin          #+#    #+#             */
/*   Updated: 2019/06/04 18:09:58 by fbecerri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_string.h"
#include "asm.h"

t_write	init_write_error(uint8_t *buffer, size_t size)
{
	t_write	error;

	error = (t_write){
		.buffer = buffer,
		.index = 0,
		.nbr_write = 0,
		.flushable = true,
		.fd = 2,
		.buffer_size = size
	};
	return (error);
}

int		print_small_error(t_read *in, t_severity severity, char *error_str
		, char *expected)
{
	t_write	error;
	uint8_t	buffer[4096];

	error = init_write_error(buffer, sizeof(buffer));
	if (severity == Err || in->werror)
	{
		severity = Err;
		in->write_able = false;
	}
	error_severity(&error, severity);
	error_msg(&error, error_str);
	if (expected)
	{
		io_write(&error, ": ", 2);
		io_write(&error, expected, ft_strlen(expected));
	}
	io_write(&error, CSI_RESET, (sizeof(CSI_RESET) - 1));
	io_write(&error, "\n", 1);
	io_flush(&error);
	return (1);
}

void	print_error(t_read *in, t_severity severity, char *erro, char *expected)
{
	size_t	i;
	t_write	error;
	uint8_t	buffer[4096];

	error = init_write_error(buffer, sizeof(buffer));
	i = 0;
	if (severity == Err || in->werror)
	{
		severity = Err;
		in->write_able = false;
	}
	error_severity(&error, severity);
	error_msg(&error, erro);
	locate_error(&error, in->begin);
	error_contxt_print(&error, (t_error){&in->begin, &in->span, &severity}, 0);
	if (expected)
	{
		io_write(&error, " ", 1);
		io_write(&error, expected, ft_strlen(expected));
	}
	io_write(&error, CSI_RESET "\n\n", (sizeof(CSI_RESET) - 1 + 2));
	io_flush(&error);
}
