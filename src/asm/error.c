/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/13 17:54:07 by prastoin          #+#    #+#             */
/*   Updated: 2019/05/05 23:03:27 by prastoin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_string.h"
#include "asm.h"

t_write	init_write_error(void)
{
	t_write	err;
	uint8_t	buffer[4096];

	err = (t_write){
	.buffer = buffer,
	.index = 0,
	.nbr_write = 0,
	.flushable = true,
	.fd = 2,
	.buffer_size = sizeof(buffer)
	};
	return (err);
}

int		print_small_error(t_read *in, t_severity severity, char *error
		, char *expected)
{
	t_write	err;

	err = init_write_error();
	if (severity == ERR || in->werror)
	{
		severity = ERR;
		in->write_able = false;
	}
	error_severity(&err, severity);
	error_msg(&err, error);
	if (expected)
	{
		io_write(&err, ": ", 2);
		io_write(&err, expected, ft_strlen(expected));
	}
	io_write(&err, CSI_RESET, (sizeof(CSI_RESET) - 1));
	io_write(&err, "\n", 1);
	io_flush(&err);
	return (1);
}

void	print_error(t_read *in, t_severity severity, char *erro, char *expected)
{
	size_t	i;
	t_write	err;

	err = init_write_error();
	i = 0;
	if (severity == ERR || in->werror)
	{
		severity = ERR;
		in->write_able = false;
	}
	error_severity(&err, severity);
	error_msg(&err, erro);
	locate_error(&err, in->begin);
	error_contxt_print(&err, (t_error){&in->begin, &in->span, &severity}, 0);
	if (expected)
	{
		io_write(&err, " ", 1);
		io_write(&err, expected, ft_strlen(expected));
	}
	io_write(&err, CSI_RESET "\n\n", (sizeof(CSI_RESET) - 1 + 2));
	io_flush(&err);
}
