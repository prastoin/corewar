/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/04 15:10:05 by prastoin          #+#    #+#             */
/*   Updated: 2019/05/04 15:17:53 by prastoin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"

bool		io_skip(t_read *rd, char e)
{
	int16_t c;

	while ((c = io_peek(rd)) != -1 && c != e)
		io_next(rd);
	if (c == -1)
		return (false);
	else
	{
		io_next(rd);
		return (true);
	}
}

bool		io_skip_until(t_read *rd, char *chars)
{
	int16_t c;
	size_t	i;

	while ((c = io_peek(rd)) != -1)
	{
		i = 0;
		while (chars[i])
		{
			if (chars[i] == c)
				return (true);
			i++;
		}
		io_next(rd);
	}
	return (false);
}

int16_t		io_peek(t_read *rd)
{
	if (rd->index == rd->len)
		if (io_fill(rd) <= 0)
			return (-1);
	return (rd->buffer[rd->index]);
}

void		io_next(t_read *rd)
{
	rd->span.col++;
	rd->span.offset++;
	if (io_peek(rd) == '\n')
	{
		rd->span.col = 1;
		rd->span.lines++;
	}
	rd->index++;
}

bool		io_expect(t_read *rd, const char *str)
{
	while (*str)
		if (io_peek(rd) == *str)
		{
			io_next(rd);
			str++;
		}
		else
			return (false);
	return (true);
}
