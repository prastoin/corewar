/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asm_header.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/30 13:46:44 by prastoin          #+#    #+#             */
/*   Updated: 2019/06/05 10:48:13 by prastoin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"

static void	asm_read_comment(t_read *rd, t_header *header)
{
	if (io_expect(rd, "."))
	{
		if (!io_expect(rd, COMMENT_CMD_STRING))
		{
			io_skip_until(rd, " #\t\n\"");
			print_error(rd, Err, "Expected \".comment\"",
				"Replace by .comment");
		}
		else
		{
			asm_skip_ws(rd);
			mark_span(rd);
			if (!asm_read_quoted(rd, header->comment, sizeof(header->comment)))
				print_error(rd, Err, "Unclosed \" for .comment", NULL);
		}
	}
	else
		print_small_error(rd, Err, ".comment not found", NULL);
}

static void	asm_read_name(t_read *rd, t_header *header)
{
	if (io_expect(rd, "."))
	{
		if (!io_expect(rd, NAME_CMD_STRING))
		{
			io_skip_until(rd, " #\t\n\"");
			print_error(rd, Err, "Expected \".name\"", "Replace by .name");
		}
		else
		{
			asm_skip_ws(rd);
			mark_span(rd);
			if (!asm_read_quoted(rd, header->name, sizeof(header->name)))
				print_error(rd, Err, "Unclosed \" for .name", NULL);
		}
	}
	else
		print_small_error(rd, Err, ".name not found", NULL);
}

t_header	asm_read_header(t_read *rd)
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
	asm_skip_ws(rd);
	return (header);
}
