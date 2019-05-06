/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asm_labels.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <dde-jesu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/26 14:19:25 by prastoin          #+#    #+#             */
/*   Updated: 2019/05/06 14:39:02 by dde-jesu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"

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
	}
	else
	{
		entry = insert_hashtable(table,
				create_entry(inst->params[i].offset.label));
		entry->resolve = false;
		entry->offset = out->nbr_write;
		inst->params[i].offset.offset = 0;
	}
	return (entry);
}

size_t	asm_resolve_label(t_hashtable **table, t_instruction *inst,
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
		if ((inst->params[i].type == PARAM_DIRECT
					|| inst->params[i].type == PARAM_INDIRECT)
						&& inst->params[i].offset.label)
		{
			entry = asm_swap_off(inst, out, i, table);
			if (!entry->resolve)
			{
				if (last_label == 1)
					last_label = 3;
				else
					last_label = i;
			}
		}
		i++;
	}
	return (last_label);
}

void	asm_store_label(t_hashtable **table, char *label, t_write *out
		, t_read *in)
{
	t_entry *entry;

	if ((entry = insert_hashtable(table, create_entry(label))))
	{
		entry->offset = out->nbr_write;
		entry->resolve = true;
	}
	else
	{
		entry = hashtable_get((*table), label);
		if (entry->resolve)
			print_error(in, WARN, "Label already exists: ", NULL);
		else
		{
			bin_resolve_label(out, entry->offset);
			entry->resolve = true;
			entry->offset = out->nbr_write;
		}
	}
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
				print_small_error(in, ERR, "Undeclared label",
						table->bucket[i].key);
			}
		}
		i++;
	}
}
