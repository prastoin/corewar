/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asm_params.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <dde-jesu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/30 13:48:34 by prastoin          #+#    #+#             */
/*   Updated: 2019/05/06 15:00:21 by dde-jesu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"

bool	asm_check_type(t_read *in, t_instruction *inst, uint16_t c, size_t i)
{
	if (c == DIRECT_CHAR)
	{
		io_next(in);
		inst->params[i].type = PARAM_DIRECT;
		asm_read_offset_value(in, inst->params + i);
	}
	else if (c == 'r')
	{
		io_next(in);
		inst->params[i].type = PARAM_REGISTER;
		inst->params[i].reg.reg = io_readnum(in);
	}
	else if (c == LABEL_CHAR || (c >= '0' && c <= '9')
			|| (c == '+' || c == '-'))
	{
		inst->params[i].type = PARAM_INDIRECT;
		asm_read_offset_value(in, inst->params + i);
	}
	else
		return (false);
	return (true);
}

void	asm_comma_check(t_read *in, t_instruction *inst, size_t i)
{
	if (g_ops[inst->opcode].params[i])
	{
		mark_span(in);
		asm_skip_ws(in);
		if (!io_expect(in, SEPARATOR_CHAR))
			print_error(in, ERR, "Expected " SEPARATOR_CHAR, NULL);
	}
}

bool	asm_read_params(t_read *in, t_instruction *inst)
{
	size_t		i;
	uint16_t	c;

	i = 0;
	while (g_ops[inst->opcode].params[i])
	{
		asm_skip_ws(in);
		mark_span(in);
		c = io_peek(in);
		if (!asm_check_type(in, inst, c, i))
		{
			io_skip_until(in, SEPARATOR_CHAR);
			print_error(in, ERR, "Invalid param",
					from_int_to_type(g_ops[inst->opcode].params[i]));
		}
		if (!(g_ops[inst->opcode].params[i] & inst->params[i].type))
		{
			print_error(in, WARN, "Type for param is invalid",
					from_int_to_type(g_ops[inst->opcode].params[i]));
			if (!g_ops[inst->opcode].ocp)
				inst->params[i].type = g_ops[inst->opcode].params[i] & 0b111;
		}
		asm_comma_check(in, inst, ++i);
	}
	return (true);
}
