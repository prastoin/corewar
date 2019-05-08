/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asm_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/26 12:46:07 by prastoin          #+#    #+#             */
/*   Updated: 2019/05/08 16:45:27 by prastoin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_string.h"
#include "asm.h"
#include <stdlib.h>

static void	asm_free_write(t_write *out, t_read *in, t_hashtable *table)
{
	bin_write_end(out);
	asm_check_labels(table, in);
	free(table);
}

void		asm_transform(t_write *out, t_read *in)
{
	t_instruction	inst;
	t_hashtable		*table;

	if (!(table = create_hashtable(8)))
		return ((void)print_small_error(in, ERR, "Table creation failed", 0));
	bin_write_header(asm_read_header(in), out);
	asm_skip_ws(in);
	while (io_peek(in) != -1 && out->fd != -1)
	{
		mark_span(in);
		if (!asm_read_inst(in, &inst))
			break ;
		if (inst.label)
			asm_store_label(&table, inst.label, out, in);
		else if (inst.opcode != -1)
			bin_write_inst(out, &inst, asm_resolve_label(&table, &inst, out
						, in));
		else
		{
			io_skip_until(in, " \t\n#");
			print_error(in, ERR, "Unknown Instructions", NULL);
		}
		asm_skip_ws(in);
	}
	asm_free_write(out, in, table);
}
