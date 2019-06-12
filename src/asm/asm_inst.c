/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asm_parse_inst.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/30 13:50:49 by prastoin          #+#    #+#             */
/*   Updated: 2019/06/12 10:25:45 by prastoin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"
#include "ft_string.h"
#include <stdlib.h>

static bool	is_labelchar(char c)
{
	size_t	i;
	size_t	len;
	char	*str;

	str = LABEL_CHARS;
	len = ft_strlen(LABEL_CHARS);
	i = 0;
	while (i < len)
	{
		if (c == str[i])
			return (true);
		i++;
	}
	return (false);
}

char		*asm_get_inst(t_read *in)
{
	size_t		len;
	int16_t		c;
	char		*str;
	char		*tmp;

	len = 0;
	if (!(str = (char*)malloc(sizeof(char) * 1)))
		return (NULL);
	while ((c = io_peek(in)) != -1 && is_labelchar(c))
	{
		str[len] = c;
		io_next(in);
		len++;
		tmp = str;
		if (!(str = realloc(str, sizeof(char) * (len + 1))))
		{
			free(tmp);
			return (NULL);
		}
	}
	str[len] = '\0';
	return (str);
}

bool		asm_read_inst(t_read *in, t_instruction *inst)
{
	char	*tmp;
	char	c;

	mark_span(in);
	if (!(tmp = asm_get_inst(in)))
		return (print_small_error(in, Err, "Malloc failed\n", 0) && false);
	c = io_peek(in);
	if (c == LABEL_CHAR)
	{
		inst->label = tmp;
		io_next(in);
	}
	else
	{
		inst->label = NULL;
		if ((inst->opcode = asm_opcode_for(tmp)) != -1)
			asm_read_params(in, inst);
		free(tmp);
	}
	return (true);
}
