/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asm_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/26 12:46:07 by prastoin          #+#    #+#             */
/*   Updated: 2019/05/22 16:38:51 by dde-jesu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_string.h"
#include "asm.h"
#include <stdlib.h>

static bool	asm_transform_end(t_write *out, t_read *in, t_hashtable *table,
		bool ret)
{
	if (ret)
	{
		bin_write_end(out);
		asm_check_labels(table, in);
	}
	free(table);
	return (ret);
}

bool		asm_transform(t_write *out, t_read *in)
{
	t_instruction	inst;
	t_hashtable		*table;
	ssize_t			idx;

	if (!(table = create_hashtable(8)))
		return (!print_small_error(in, ERR, "Table creation failed", 0));
	bin_write_header(asm_read_header(in), out);
	while (io_peek(in) != -1 && out->fd != -1)
	{
		mark_span(in);
		if (!asm_read_inst(in, &inst)
			|| (inst.label && !asm_store_label(&table, inst.label, out, in)))
			return (asm_transform_end(out, in, table, false));
		if (!inst.label && inst.opcode != -1)
		{
			if ((idx = asm_resolve_label(&table, &inst, out, in)) < 0)
				return (asm_transform_end(out, in, table, false));
			bin_write_inst(out, &inst, idx);
		}
		if (!inst.label && inst.opcode == -1 && io_skip_until(in, " \t\n#") | 1)
			print_error(in, ERR, "Unknown Instructions", NULL);
		asm_skip_ws(in);
	}
	return (asm_transform_end(out, in, table, out->fd != -1));
}
