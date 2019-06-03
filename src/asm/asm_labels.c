/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asm_labels.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <dde-jesu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/26 14:19:25 by prastoin          #+#    #+#             */
/*   Updated: 2019/06/03 11:51:56 by dde-jesu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"
#include <stdlib.h>

t_entry	*asm_swap_off(t_instruction *inst, t_write *out, size_t i,
		t_hashtable **table)
{
	t_entry *entry;

	if ((entry = hashtable_get((*table), inst->params[i].offset.label)))
	{
		inst->params[i].offset.offset = entry->offset;
		if (!entry->resolve)
			entry->offset = out->nbr_write;
		else
			inst->params[i].offset.offset -= (ssize_t)out->nbr_write;
		free(inst->params[i].offset.label);
	}
	else if ((entry = insert_hashtable(table,
				create_entry(inst->params[i].offset.label))))
	{
		entry->resolve = false;
		entry->offset = out->nbr_write;
		inst->params[i].offset.offset = 0;
	}
	else
		free(inst->params[i].offset.label);
	return (entry);
}

ssize_t	asm_resolve_label(t_hashtable **table, t_instruction *inst,
		t_write *out, t_read *in)
{
	size_t	i;
	size_t	last_label;
	t_entry	*entry;

	i = 0;
	last_label = 0;
	while (g_ops[inst->opcode].params[i])
	{
		mark_span(in);
		if ((inst->params[i].type == Param_Direct
					|| inst->params[i].type == Param_Indirect)
						&& inst->params[i].offset.label)
		{
			if (!(entry = asm_swap_off(inst, out, i, table)))
			{
				print_small_error(in, Err, "Malloc failed\n", 0);
				return (-1);
			}
			if (!entry->resolve)
				last_label = last_label == 1 ? 3 : i;
		}
		i++;
	}
	return (last_label);
}

bool	asm_store_label(t_hashtable **table, char *label, t_write *out,
		t_read *in)
{
	t_entry *entry;

	if ((entry = insert_hashtable(table, create_entry(label))))
	{
		entry->offset = out->nbr_write;
		entry->resolve = true;
	}
	else if ((entry = hashtable_get((*table), label)))
	{
		if (entry->resolve)
			print_error(in, Warn, "Label already exists: ", NULL);
		else
		{
			bin_resolve_label(out, entry->offset);
			entry->resolve = true;
			entry->offset = out->nbr_write;
		}
		free(label);
	}
	else
	{
		print_small_error(in, Err, "Malloc failed\n", 0);
		free(label);
		return (false);
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
			if (!table->bucket[i].resolve)
			{
				print_small_error(in, Err, "Undeclared label",
						table->bucket[i].key);
			}
			free(table->bucket[i].key);
		}
		i++;
	}
}
