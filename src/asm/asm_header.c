/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asm_header.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/30 13:46:44 by prastoin          #+#    #+#             */
/*   Updated: 2019/04/30 14:52:35 by prastoin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"

static void	asm_read_comment(t_read *rd, t_header *header)
{
	if (io_expect(rd, "."))
	{
		if (!io_expect(rd, "comment"))
		{
			io_skip_until(rd, " #\t\n\"");
			print_error(rd, ERR, "Expected \".comment\"", "Replace by .comment");
		}
		else
		{
			asm_skip_ws(rd);
			mark_span(rd);
			if (!asm_read_quoted(rd, header->comment, sizeof(header->comment)))
				print_error(rd, ERR, "Unclosed \" for .comment", NULL);
		}
	}
	else
		print_small_error(rd, ERR, ".comment not found");
}

static void	asm_read_name(t_read *rd, t_header *header)
{
	if (io_expect(rd, "."))
	{
		if (!io_expect(rd, "name"))
		{
			io_skip_until(rd, " #\t\n\"");
			print_error(rd, ERR, "Expected \".name\"", "Replace by .name");
		}
		else
		{
			asm_skip_ws(rd);
			mark_span(rd);
			if (!asm_read_quoted(rd, header->name, sizeof(header->name)))
				print_error(rd, ERR, "Unclosed \" for .name", NULL);
		}
	}
	else
		print_small_error(rd, ERR, ".name not found");
}

t_header		asm_read_header(t_read *rd)
{
	t_header		header;

	header = (t_header) {
		.size = 0
	};
	asm_skip_ws(rd);
	mark_span(rd);
	asm_read_name(rd, &header);
	asm_skip_ws(rd);
	mark_span(rd);
	asm_read_comment(rd, &header);
	return (header);
}

