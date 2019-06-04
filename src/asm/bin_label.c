/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bin_label.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/04 15:25:39 by prastoin          #+#    #+#             */
/*   Updated: 2019/06/04 12:27:55 by prastoin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"
#include <stdlib.h>

void		bin_resolve_positions(t_write *out, t_pos_vec *positions)
{
	size_t	src;
	size_t	i;
	t_pos	*pos;

	i = 0;
	src = out->nbr_write;
	while (i < positions->len)
	{
		pos = positions->inner + i;
		io_seek(out, pos->param, true);
		io_write_int(out, out->nbr_write - (ssize_t)pos->offset, pos->size);
		out->nbr_write = src;
		i++;
	}
	free(positions);
	io_seek(out, src, true);
	out->nbr_write = src;
}
