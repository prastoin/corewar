/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asm_read.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/14 11:34:39 by prastoin          #+#    #+#             */
/*   Updated: 2019/03/14 15:50:43 by prastoin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_string.h"
#include "asm.h"

bool	asm_skip_ws(t_read *rd)
{
	int16_t		c;

	while ((c = io_peek(rd)) == ' ' || c == '\t' || c == '#' || c == '\n')
	{
		rd->index++;
		if (c == '#')
			if (!io_skip(rd , '\n'))
				return (false);
	}
	return (true);
}

bool		asm_read_quoted(t_read *rd, char data[], size_t len)
{
	size_t		i;
	int16_t		c;
	
	i = 0;
	if (!(io_expect(rd, "\"")))
		return (false);
	while ((c = io_peek(rd)) != '"' && c != -1)
	{
		if (c == '\\')
		{
			rd->index++;
			if ((c = io_peek(rd)) == -1)
				return (false);
		}
		data[i] = c;
		rd->index++;
		i++;
		if (i >= len)
		{
			io_skip(rd, '"');
			return (false);
		}
	}
	if (i < len)
		data[i] = '\0';
	if (c == '"')
		rd->index++;
	return (c != -1);
}

/*bool		asm_fill_header(t_write *out, t_header *head)
{
	size_t i;

	i = 0;
	asm_fill_magic();
	while (head->name[i] || i < PROG_NAME_LENGTH)
	{
		out->buffer[out->index] = head->name[i];
		out->index++;
		i++;
	}
	return (true);
}*/

size_t		asm_opcode_for(char *name)
{
	size_t i;

	i = 0;
	while (ft_strcmp(name, g_ops[i].name) != 0 && i < REG_NUMBER)
		i++;
	return (i < REG_NUMBER ? i : -1);
}

bool		asm_parse_header(t_read *rd, t_header *header)
{
	header->size = 0;
	asm_skip_ws(rd);
	io_expect(rd, ".name");
	asm_skip_ws(rd);
	asm_read_quoted(rd, header->name, sizeof(header->name));
	asm_skip_ws(rd);
	io_expect(rd, ".comment");
	asm_skip_ws(rd);
	asm_read_quoted(rd, header->comment, sizeof(header->comment));
	return (true);
}

bool		asm_parse_instruction(t_read *in, t_instruction *inst)
{
	char	*tmp;
	char	c;
	size_t	i;

	i = 0;
	if (!(tmp = (char *)malloc(sizeof(char) * 1)))
		return (false);
	while ((c = io_peek(in)) != -1 && c != ' ' && c != '\t'
			&& c != '\n' && c != ':')
	{
		tmp[i] = c;
		i++;
		if (!(tmp = realloc(tmp, i + 1)))
			return (false);
	}
	tmp[i] = '\0';
	if (c == ':')
		inst->label = tmp;
	else
		
	asm_skip_ws(in);

}


int main(int argc, const char *argv[])
{
	t_read			in;
	t_header		head;
	t_write			out;
	t_instruction	inst;

	in = init_read(open(argv[1], O_RDONLY));
	asm_parse_header(&in, &head);
	while (asm_parse_instruction(&in, &inst))
	{
		asm_write_inst(&out, &inst);
	}
	return (0);
}
