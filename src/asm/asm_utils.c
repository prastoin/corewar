/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asm_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/26 13:23:44 by prastoin          #+#    #+#             */
/*   Updated: 2019/04/26 14:29:33 by prastoin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_string.h"
#include "asm.h"

bool	asm_skip_ws(t_read *rd)
{
	int16_t		c;

	while ((c = io_peek(rd)) == ' ' || c == '\t' || c == '#' || c == '\n')
	{
		io_next(rd);
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
	while ((c = io_peek(rd)) != '"' && c != '\n' && c != -1)
	{
		if (c == '\\')
		{
			io_next(rd);
			if ((c = io_peek(rd)) == -1)
				return (false);
		}
		data[i] = c;
		io_next(rd);
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
		io_next(rd);
	return (c == '"');
}

ssize_t		asm_opcode_for(char *name)
{
	size_t i;

	i = 1;
	while (i < 17 && ft_strcmp(name, g_ops[i].name) != 0)
		i++;
	return (i < 17 ? i : -1);
}

void		asm_read_offset_value(t_read *in, t_param *param)
{
	uint16_t	c;

	c = io_peek(in);
	if (c == LABEL_CHAR)
	{
		io_next(in);
		param->offset.label = asm_parse_name(in);
	}
	else
	{
		param->offset.offset = io_readnum(in);
		param->offset.label = NULL;
	}
}
