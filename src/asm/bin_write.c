/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bin_write.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <dde-jesu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/19 11:24:17 by prastoin          #+#    #+#             */
/*   Updated: 2019/06/05 10:14:18 by prastoin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"
#include "ft_string.h"

void	bin_write_params(t_write *out, t_instruction *inst)
{
	size_t	i;

	i = 0;
	while (g_ops[inst->opcode].params[i])
	{
		if (inst->params[i].type == Param_Direct)
			io_write_int(out, inst->params[i].offset.offset,
				g_ops[inst->opcode].params[i] & Param_Index ? 2 : 4);
		else if (inst->params[i].type == Param_Indirect)
			io_write_int(out, inst->params[i].offset.offset, 2);
		else if (inst->params[i].type == Param_Register)
			io_write_int(out, inst->params[i].reg.reg, 1);
		i++;
	}
}

void	bin_write_inst(t_write *out, t_instruction *inst)
{
	size_t			i;
	uint8_t			ocp;

	ocp = 0;
	i = 0;
	io_write_int(out, inst->opcode, 1);
	while (g_ops[inst->opcode].params[i])
	{
		if (inst->params[i].type == Param_Direct)
			ocp |= 0b10 << ((3 - i) * 2);
		else if (inst->params[i].type == Param_Indirect)
			ocp |= 0b11 << ((3 - i) * 2);
		else if (inst->params[i].type == Param_Register)
			ocp |= 0b01 << ((3 - i) * 2);
		i++;
	}
	if (g_ops[inst->opcode].ocp == true)
		io_write_int(out, ocp, 1);
	bin_write_params(out, inst);
}

void	bin_write_end(t_write *out)
{
	size_t		len;

	if (out->flushable)
		io_flush(out);
	len = 4 + sizeof(((t_header *)0)->name) + 4
		- sizeof(((t_header *)0)->name) % 4;
	io_seek(out, len, true);
	len += 4 + sizeof(((t_header *)0)->comment) + 4
		- sizeof(((t_header *)0)->comment) % 4;
	io_write_int(out, out->nbr_write - len, 4);
	if (out->flushable)
		io_flush(out);
	out->nbr_write -= 4;
}

bool	bin_write_header(t_header head, t_write *out)
{
	io_write_int(out, COREWAR_EXEC_MAGIC, 4);
	io_write(out, head.name, sizeof(head.name));
	io_write(out, (uint8_t[4]){0, 0, 0, 0}, 4 - sizeof(head.name) % 4);
	io_write_int(out, 0, 4);
	io_write(out, head.comment, sizeof(head.comment));
	io_write(out, (uint8_t[4]){0, 0, 0, 0}, 4 - sizeof(head.comment) % 4);
	return (true);
}

void	bin_padding_ocp(uint8_t ocp, t_write *out, int8_t *size,
		uint8_t opcode)
{
	uint8_t i;
	uint8_t type;

	(*size) = 0;
	i = 0;
	while (i < (ocp & 0b11) && i < 2)
	{
		type = (ocp >> ((3 - i) * 2)) & 0b11;
		if (type == 0b01)
			(*size) += 1;
		else if (type == 0b10)
			(*size) += g_ops[opcode].params[i] & Param_Index ? 2 : 4;
		else if (type == 0b11)
			(*size) += 2;
		i++;
	}
	ocp &= 0b11111100;
	io_seek(out, -1, false);
	io_write(out, &ocp, 1);
	io_seek(out, (*size), false);
	(*size) = 2;
	type = (ocp >> ((3 - i) * 2)) & 0b11;
	if (type == 0b10 && (!(g_ops[opcode].params[i] & Param_Index)))
		(*size) = 4;
}
