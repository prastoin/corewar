/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/29 10:28:54 by prastoin          #+#    #+#             */
/*   Updated: 2019/05/22 22:21:56 by prastoin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"

void	mark_span(t_read *in)
{
	in->begin = in->span;
}

bool	escape(t_read *rd, int16_t c)
{
	io_next(rd);
	if ((c = io_peek(rd)) == -1)
		return (false);
	return (true);
}
