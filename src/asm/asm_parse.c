/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asm_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/26 12:46:07 by prastoin          #+#    #+#             */
/*   Updated: 2019/04/29 11:24:44 by prastoin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common.h"
#include "asm.h"

bool		asm_parse_header(t_read *rd, t_header *header)
{
	header->size = 0;
	asm_skip_ws(rd);
	mark_span(rd);
	if (io_expect(rd, "."))
	{
		if (!io_expect(rd, "name"))
		{
			io_skip_until(rd, " #\t\n\"");
			print_error(rd, 1, "Expected \".name\"", "Replace by .name");
		}
		else
		{
			asm_skip_ws(rd);
			mark_span(rd);
			if (!asm_read_quoted(rd, header->name, sizeof(header->name)))
				print_error(rd, 1, "Unclosed \" for .name", NULL);
		}
	}
	else
		print_small_error(rd, 1, ".name not found");
	asm_skip_ws(rd);
	mark_span(rd);
	if (io_expect(rd, "."))
	{
		if (!io_expect(rd, "comment"))
		{
			io_skip_until(rd, " #\t\n\"");
			print_error(rd, 1, "Expected \".comment\"", "Replace by .comment");
		}
		else
		{
			asm_skip_ws(rd);
			mark_span(rd);
			if (!asm_read_quoted(rd, header->comment, sizeof(header->comment)))
				print_error(rd, 1, "Unclosed \" for .comment", NULL);
		}
	}
	else
		print_small_error(rd, 1, ".comment not found");
	return (true);
}

char		*asm_parse_name(t_read *in)
{
	size_t		len;
	int16_t		c;
	char		*str;

	len = 0;
	if (!(str = (char*)malloc(sizeof(char) * 1)))
		return (NULL);
	while ((c = io_peek(in)) != -1 && ((c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '_'))
	{
		str[len] = c;
		io_next(in);
		len++;
		if (!(str = realloc(str, sizeof(char) * (len + 1))))
			return (NULL);
	}
	str[len] = '\0';
	if (c == -1)
		return (NULL);
	return (str);
}

bool		asm_parse_params(t_read *in, t_instruction *inst)
{
	size_t		i;
	uint16_t	c;

	i = 0;
	while (g_ops[inst->opcode].params[i])
	{
		asm_skip_ws(in);
		in->begin = in->span;
		c = io_peek(in);
		if (c == DIRECT_CHAR)
		{
			io_next(in);
			inst->params[i].type = PARAM_DIRECT;
			asm_read_offset_value(in, inst->params + i);
		}
		else if (c == 'r')
		{
			io_next(in);
			inst->params[i].type = PARAM_REGISTER;
			inst->params[i].reg.reg = io_readnum(in);
		}
		else if (c == LABEL_CHAR || (c >= '0' && c <= '9')
				|| (c == '+' || c == '-'))
		{
			inst->params[i].type = PARAM_INDIRECT;
			asm_read_offset_value(in, inst->params + i);
		}
		else
		{
			io_skip_until(in, SEPARATOR_CHAR);
			print_error(in, 1, "Invalid param", from_int_to_type(g_ops[inst->opcode].params[i]));
		}
		if (!(g_ops[inst->opcode].params[i] & inst->params[i].type))
			print_error(in, 2, "Type for param is invalid", from_int_to_type(g_ops[inst->opcode].params[i]));
		i++;
		if (g_ops[inst->opcode].params[i])
		{
			in->begin = in->span;
			asm_skip_ws(in);
			if (!io_expect(in, SEPARATOR_CHAR))
				print_error(in, 1, "Expected " SEPARATOR_CHAR, NULL);
		}
	}
	return (true);
}

