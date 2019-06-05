/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asm_labels2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/04 13:23:46 by prastoin          #+#    #+#             */
/*   Updated: 2019/06/05 10:24:57 by prastoin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"
#include <stdlib.h>

void	fill_pos(t_pos *pos, t_write *out, t_instruction *inst, size_t i)
{
	pos->param = out->nbr_write + size_until_param(inst, i);
	pos->offset = out->nbr_write;
	if (inst->params[i].type == Param_Direct)
		pos->size = g_ops[inst->opcode].params[i] & Param_Index ? 2 : 4;
	else
		pos->size = 2;
}

bool	free_inst_label(t_instruction *inst, size_t i)
{
	free(inst->params[i].offset.label);
	return (false);
}

bool	check_entry_position(t_entry **entry, t_pos **pos)
{
	if (!((*entry)->positions = create_pos_vec(8))
		|| (!((*pos) = add_position(&((*entry)->positions)))))
		return (false);
	return (true);
}

void	fix_new_offset(t_entry *entry, t_instruction *inst,
		size_t i, t_write *out)
{
	inst->params[i].offset.offset = entry->offset
		- (ssize_t)out->nbr_write;
}
