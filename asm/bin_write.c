/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asm_write.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/19 11:24:17 by prastoin          #+#    #+#             */
/*   Updated: 2019/03/26 10:47:52 by prastoin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"
#include "ft_string.h"

void	io_flush(t_write *out)
{
	write(out->fd, &out->buffer, out->index);
	out->index = 0;
}

void	io_write(t_write *out, uint8_t c)
{
	out->buffer[out->index] = c;
	out->index++;
	out->nbr_write++;
	if (out->index == BUFFER_SIZE)
		io_flush(out);
}

void	io_write_int(t_write *out, uintmax_t nb, size_t nb_bytes)
{
	while (nb_bytes != 0)
	{
		nb_bytes--;
		io_write(out, (nb >> (nb_bytes * 8)) & 0xFF);
	}
}

void		bin_write_inst(t_write *out, t_instruction *inst, uint8_t last_label)
{
	size_t			i;
	static	int		lb;
	uint8_t			ocp;

	ocp = last_label & 0b11;
	i = 0;
	io_write_int(out, inst->opcode, 1);
	while (g_ops[inst->opcode].params[i])
	{
		if (inst->params[i].type == PARAM_DIRECT)
			ocp |= 0b10 << ((3 - i) * 2);
		else if (inst->params[i].type == PARAM_INDIRECT)
			ocp |= 0b11 << ((3 - i) * 2);
		else if (inst->params[i].type == PARAM_REGISTER)
			ocp |= 0b01 << ((3 - i) * 2);
		i++;
	}
	if (i > 1)
		io_write_int(out, ocp, 1);
	i = 0;
	while (g_ops[inst->opcode].params[i])
	{

		if (inst->params[i].type == PARAM_DIRECT)
			io_write_int(out, inst->params[i].direct.offset, g_ops[inst->opcode].params[i] & PARAM_INDEX ? 2 : 4);
		else if (inst->params[i].type == PARAM_INDIRECT)
			io_write_int(out, inst->params[i].indirect.offset, 2);
		else if (inst->params[i].type == PARAM_REGISTER)
			io_write_int(out, inst->params[i].reg.reg, 1);
		i++;
	}
}

void	bin_write_end(t_write *out, t_header *head)
{
	size_t	len;
	size_t	nb;

	io_flush(out);
	len = 4 + (sizeof(head->name) + 1) + ((4 - (sizeof(head->name) + 1) % 4) % 4);
	lseek(out->fd, len, SEEK_SET);
	len += 4 + (sizeof(head->comment) + 1) + ((4 - (sizeof(head->comment) + 1) % 4) % 4);
	nb = out->nbr_write - len;
	io_write_int(out, nb, 4);
	io_flush(out);
}

bool	write_header(t_header *head, t_write *out)
{
	size_t		padd;
	size_t		i;
	size_t		len;

	io_write_int(out, COREWAR_EXEC_MAGIC, 4);
	padd = (sizeof(head->name));
	padd += (4 - (sizeof(head->name) % 4));
	i = 0;
	len = ft_strlen(head->name);
	while (i < padd)
	{
		if (i < PROG_NAME_LENGTH && i < len)
			io_write(out, head->name[i]);
		else
			io_write(out, '\0');
		i++;
	}
	io_write_int(out, 0, 4);
	i = 0;
	padd = sizeof(head->comment);
	padd += (4 - (sizeof(head->comment) % 4));
	len = ft_strlen(head->comment);
	while (i < padd)
	{
		if (i < COMMENT_LENGTH && i < len)
			io_write(out, head->comment[i]);
		else
			io_write(out, '\0');
		i++;
	}
	return (true);
}

void		bin_resolve_label(t_write *out, size_t offset)
{
	ssize_t	src;
	uint8_t	opcode;
	uint8_t	ocp;
	uint8_t	tmp[4];
	int8_t	size;
	size_t	i;
	size_t save_ocp;
	size_t save_off;

	io_flush(out);
	src = out->nbr_write;
	while (offset != 0x0000)
	{
		lseek(out->fd, offset, SEEK_SET);
		read(out->fd, &opcode, 1);
		if (g_ops[opcode].params[1])
		{
			size = 0;
			read(out->fd, &ocp, 1);
			i = 0;
			while (i < (ocp & 0b11) && i < 2)
			{
				uint8_t type = (ocp >> ((3 - i) * 2)) & 0b11;
				if (type == 0b01)
					size += 1;
				else if (type == 0b10)
					size += g_ops[opcode].params[i] & PARAM_INDEX ? 2 : 4;
				else if (type == 0b11)
					size += 2;
				i++;
			}
			save_ocp = ocp;
			ocp &= 0b11111100;
			lseek(out->fd, -1, SEEK_CUR);
			write(out->fd, &ocp, 1);
			lseek(out->fd, size, SEEK_CUR);
			size = 2;
			uint8_t type = (ocp >> ((3 - i) * 2)) & 0b11;
			if (type == 0b10 && (!(g_ops[opcode].params[i] & PARAM_INDEX)))
				size = 4;
		}
		else
		{
			size = 2;
			if (g_ops[opcode].params[0] & PARAM_DIRECT && (!(g_ops[opcode].params[0] & PARAM_INDEX)))
				size = 4;
		}
		read(out->fd, tmp, size);
		lseek(out->fd, -size, SEEK_CUR);
		io_write_int(out, src - (ssize_t)offset, size);
		io_flush(out);
		save_off = offset;
		if (size == 2)
			offset = tmp[0] * 0x100 + tmp[1];
		else if (size == 4)
			offset = tmp[0] * 0x1000000 + tmp[1] * 0x10000 + tmp[2] * 0x100 + tmp[3];
		if (save_off == offset)
		{
			lseek(out->fd, (save_off + 1), SEEK_SET);
			if ((save_ocp & 0b11) >= 2)
				save_ocp -= 2;
			else
				save_ocp -= 1;
			ocp = save_ocp;
			write(out->fd, &ocp, 1);
		}
	}
	lseek (out->fd, 0, SEEK_END);
	out->nbr_write = src;
}
