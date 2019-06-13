/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asm_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/26 12:46:07 by prastoin          #+#    #+#             */
/*   Updated: 2019/06/13 10:19:16 by dde-jesu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"
#include <stdlib.h>

static bool	asm_transform_end(t_write *out, t_read *in, t_hashtable *table,
		bool ret)
{
	asm_check_labels(table, in);
	bin_write_end(out);
	free(table);
	return (ret);
}

bool		asm_transform(t_write *out, t_read *in)
{
	t_instruction	inst;
	t_hashtable		*table;

	if (!(table = create_hashtable(8)))
		return (!print_small_error(in, Err, "Table creation failed", 0));
	bin_write_header(asm_read_header(in), out);
	while (io_peek(in) != -1 && out->fd != -1)
	{
		mark_span(in);
		if (!asm_read_inst(in, &inst)
			|| (inst.label && !asm_store_label(&table, inst.label, out, in)))
			return (asm_transform_end(out, in, table, false));
		if (!inst.label && inst.opcode != -1)
		{
			if (!asm_resolve_label(&table, &inst, out, in))
				return (asm_transform_end(out, in, table, false));
			bin_write_inst(out, &inst);
		}
		if (!inst.label && inst.opcode == -1 && io_skip_until(in, " \t\n#") | 1)
			print_error(in, Err, "Unknown Instructions", NULL);
		asm_skip_ws(in);
	}
	return (asm_transform_end(out, in, table, out->fd != -1));
}
