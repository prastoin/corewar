/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bin_label.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/04 15:25:39 by prastoin          #+#    #+#             */
/*   Updated: 2019/05/04 15:38:18 by prastoin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"

size_t		bin_multi_label_case(size_t offset, int8_t size, t_write *out,
		int8_t ocp)
{
	size_t	save_off;
	uint8_t	tmp[4];

	io_write_read(out, tmp, size);
	io_seek(out, -size, false);
	io_write_int(out, out->nbr_write - (ssize_t)offset, size);
	save_off = offset;
	if (size == 2)
		offset = tmp[0] * 0x100 + tmp[1];
	else if (size == 4)
		offset = tmp[0] * 0x1000000 + tmp[1] * 0x10000 + tmp[2] * 0x100
			+ tmp[3];
	if (save_off == offset)
	{
		io_seek(out, (save_off + 1), true);
		if ((ocp & 0b11) >= 2)
			ocp -= 2;
		else
			ocp -= 1;
		io_write(out, &ocp, 1);
	}
	return (offset);
}

void		bin_resolve_label(t_write *out, size_t offset)
{
	ssize_t	src;
	uint8_t	opcode;
	uint8_t	ocp;
	int8_t	size;

	src = out->nbr_write;
	while (offset != 0x0000)
	{
		io_seek(out, offset, true);
		io_write_read(out, &opcode, 1);
		if (g_ops[opcode].ocp == true)
		{
			io_write_read(out, &ocp, 1);
			bin_padding_ocp(ocp, out, &size, opcode);
		}
		else
			size = g_ops[opcode].params[0] & PARAM_DIRECT
				&& (!(g_ops[opcode].params[0] & PARAM_INDEX)) ? 4 : 2;
		out->nbr_write = src;
		offset = bin_multi_label_case(offset, size, out, ocp);
	}
	io_seek(out, src, true);
	out->nbr_write = src;
}
