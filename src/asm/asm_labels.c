/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asm_labels.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <dde-jesu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/26 14:19:25 by prastoin          #+#    #+#             */
/*   Updated: 2019/06/04 12:26:22 by prastoin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"
#include <stdlib.h>

size_t	size_until_param(t_instruction *inst, size_t pos)
{
	size_t	i;
	size_t	size;

	i = 0;
	size = 0;
	while (i < pos)
	{
		if (inst->params[i].type == Param_Direct)
			size += g_ops[inst->opcode].params[i] & Param_Index ? 2 : 4;
		else if (inst->params[i].type == Param_Indirect)
			size += 2;
		else if (inst->params[i].type == Param_Register)
			size += 1;
		i++;
	}
	return (1 + g_ops[inst->opcode].ocp + size);
}

bool asm_swap_off(t_instruction *inst, t_write *out, size_t i,
		t_hashtable **table)
{
	t_entry *entry;
	t_pos	*pos;

	if ((entry = hashtable_get((*table), inst->params[i].offset.label)))
	{
		if (entry->positions)
			inst->params[i].offset.offset -= (ssize_t)out->nbr_write;
		else
		{
			if (!(pos = add_position(&entry->positions)))
				return (false);
			pos->param = out->nbr_write + size_until_param(inst, i);
			pos->offset = out->nbr_write;
			if (inst->params[i].type == Param_Direct)
				pos->size = g_ops[inst->opcode].params[i] & Param_Index ? 2 : 4;
			else
				pos->size = 2;
		}
		free(inst->params[i].offset.label);
	}
	else if ((entry = insert_hashtable(table,
					create_entry(inst->params[i].offset.label))))
	{
		entry->positions = create_pos_vec(8);
		if (!(pos = add_position(&entry->positions)))
			return (false);
		pos->param = out->nbr_write + size_until_param(inst, i);
		pos->offset = out->nbr_write;
		if (inst->params[i].type == Param_Direct)
			pos->size = g_ops[inst->opcode].params[i] & Param_Index ? 2 : 4;
		else
			pos->size = 2;
	}
	else
	{
		free(inst->params[i].offset.label);
		return (false);
	}
	return (true);
}

bool	asm_resolve_label(t_hashtable **table, t_instruction *inst,
		t_write *out, t_read *in)
{
	size_t	i;

	i = 0;
	while (g_ops[inst->opcode].params[i])
	{
		mark_span(in);
		if ((inst->params[i].type == Param_Direct
				|| inst->params[i].type == Param_Indirect)
			&& inst->params[i].offset.label)
		{
			if (!asm_swap_off(inst, out, i, table))
			{
				print_small_error(in, Err, "Malloc failed\n", 0);
				return (false);
			}
		}
		i++;
	}
	return (true);
}

bool	asm_store_label(t_hashtable **table, char *label, t_write *out,
		t_read *in)
{
	t_entry *entry;

	if ((entry = insert_hashtable(table, create_entry(label))))
	{
		entry->offset = out->nbr_write;
		entry->positions = NULL;
	}
	else if ((entry = hashtable_get((*table), label)))
	{
		if (entry->positions)
		{
			bin_resolve_positions(out, entry->positions);
			entry->positions = NULL;
			entry->offset = out->nbr_write;
		}
		else
			print_error(in, Warn, "Label already exists: ", NULL);
		free(label);
	}
	else
	{
		free(label);
		return (!(print_small_error(in, Err, "Malloc failed\n", 0)));
	}
	return (true);
}

void	asm_check_labels(t_hashtable *table, t_read *in)
{
	size_t	i;

	i = 0;
	while (i < table->size)
	{
		if (table->bucket[i].key)
		{
			if (table->bucket[i].positions)
			{
				print_small_error(in, Err, "Undeclared label",
					table->bucket[i].key);
				free(table->bucket[i].positions);
			}
			free(table->bucket[i].key);
		}
		i++;
	}
}
