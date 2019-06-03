/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asm_params.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <dde-jesu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/30 13:48:34 by prastoin          #+#    #+#             */
/*   Updated: 2019/05/22 22:34:45 by prastoin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"

bool	asm_check_type(t_read *in, t_instruction *inst, uint16_t c, size_t i)
{
	inst->params[i] = (t_param) {
		.type = Param_None
	};
	if (c == DIRECT_CHAR)
	{
		io_next(in);
		inst->params[i].type = Param_Direct;
		asm_read_offset_value(in, inst->params + i);
	}
	else if (c == 'r')
	{
		io_next(in);
		inst->params[i].type = Param_Register;
		inst->params[i].reg.reg = io_readnum(in);
	}
	else if (c == LABEL_CHAR || (c >= '0' && c <= '9')
		|| (c == '+' || c == '-'))
	{
		inst->params[i].type = Param_Indirect;
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
			print_error(in, Err, "Expected " SEPARATOR_CHAR, NULL);
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
			print_error(in, Err, "Invalid param",
				from_int_to_type(g_ops[inst->opcode].params[i]));
		}
		if (!(g_ops[inst->opcode].params[i] & inst->params[i].type))
		{
			print_error(in, Warn, "Type for param is invalid",
				from_int_to_type(g_ops[inst->opcode].params[i]));
			if (!g_ops[inst->opcode].ocp)
				inst->params[i].type = g_ops[inst->opcode].params[i] & 0b111;
		}
		asm_comma_check(in, inst, ++i);
	}
	return (true);
}
