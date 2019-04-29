/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asm_content_parse.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/26 14:19:25 by prastoin          #+#    #+#             */
/*   Updated: 2019/04/29 10:20:51 by prastoin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"

bool		asm_parse_instruction(t_read *in, t_instruction *inst)
{
	char	*tmp;
	char	c;
	size_t	i;
	const t_span begin = in->span;

	i = 0;
	if (!(tmp = asm_parse_name(in)))
		return (false);
	c = io_peek(in); //TODO protect return
	if (c == ':')
	{
		inst->label = tmp;
		io_next(in);
	}
	else
	{
		inst->label = NULL;
		if ((inst->opcode = asm_opcode_for(tmp)) != -1)
			asm_parse_params(in, inst);
		else
		{
			io_skip_until(in, " \t\n#");
			print_error(1, begin, in->span, "Unknown Instructions", NULL);
		}
		free(tmp);
	}
	return (true);
}

void		gest_arg(t_instruction inst, t_hashtable **table, t_write *out, t_read *in)
{
	size_t i;
	size_t last_label;
	t_entry *entry;

	i = 0;
	last_label = 0;
	while (g_ops[inst.opcode].params[i])
	{
		in->begin = in->span;
		if ((inst.params[i].type == PARAM_DIRECT || inst.params[i].type == PARAM_INDIRECT)
				&& inst.params[i].offset.label)
		{
			if ((entry = hashtable_get((*table), inst.params[i].offset.label)))
			{
				inst.params[i].offset.offset = entry->offset;
				if (!entry->resolve)
					entry->offset = out->nbr_write;
				else
					inst.params[i].offset.offset -= (ssize_t)out->nbr_write;
			}
			else
			{
				entry = insert_hashtable(table, create_entry(inst.params[i].offset.label));
				entry->resolve = false;
				entry->offset = out->nbr_write;
				inst.params[i].offset.offset = 0;
			}
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
	bin_write_inst(out, &inst, last_label);
}

void		case_label(t_hashtable **table, t_instruction inst, t_write *out, t_read *in)
{
	t_entry *entry;

	if ((entry = insert_hashtable(table, create_entry(inst.label))))
	{
		entry->offset = out->nbr_write;
		entry->resolve = true;
	}
	else
	{
		entry = hashtable_get((*table), inst.label);
		if (entry->resolve)
			print_error(2, in->begin, in->span, "Label already exists: ", NULL);
		else
		{
			bin_resolve_label(out, entry->offset);
			entry->resolve = true;
			entry->offset = out->nbr_write;
		}
	}
}

bool	asm_parser(t_write *out, t_read *in, t_hashtable *table)
{
	t_instruction	inst;

	ft_header(out, in); //return to manage
	in->begin = in->span;
	while (io_peek(in) != -1)
	{
		asm_skip_ws(in);
		in->begin = in->span;
		if (!asm_parse_instruction(in, &inst))
			break ;
		if (inst.label)
			case_label(&table, inst, out, in);
		else if (inst.opcode != -1)
			gest_arg(inst, &table, out, in);
		else
		{
			// TODO gestion de l'erreur
		}
	}
	bin_write_end(out);
	return (true);
}
