/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asm_parse_inst.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/30 13:50:49 by prastoin          #+#    #+#             */
/*   Updated: 2019/04/30 15:03:01 by prastoin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"

bool		asm_read_inst(t_read *in, t_instruction *inst)
{
	char	*tmp;
	char	c;
	size_t	i;

	mark_span(in);
	i = 0;
	if (!(tmp = asm_get_inst(in)))
		return (false);
	c = io_peek(in);
	if (c == ':')
	{
		inst->label = tmp;
		io_next(in);
	}
	else
	{
		inst->label = NULL;
		if ((inst->opcode = asm_opcode_for(tmp)) != -1)
			asm_read_params(in, inst);
		free(tmp);
	}
	return (true);
}

