/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asm_read.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/14 11:34:39 by prastoin          #+#    #+#             */
/*   Updated: 2019/03/14 12:31:26 by prastoin         ###   ########.fr       */
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

int main(int argc, const char *argv[])
{
	t_read		in;
	t_header	head;
	t_write		out;

	in = init_read(open(argv[1], O_RDONLY));
	asm_parse_header(&in, &head);
	asm_write_header(&out, &head);
	return (0);
}
