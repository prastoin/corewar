/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io_write.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/04 15:09:06 by prastoin          #+#    #+#             */
/*   Updated: 2019/06/04 18:14:15 by fbecerri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"

void	io_write_number(t_write *w, uintmax_t n)
{
	uintmax_t	tmp;
	uintmax_t	pow;
	char		c;

	tmp = n;
	pow = 1;
	while (tmp /= 10)
		pow *= 10;
	while (pow)
	{
		c = n / pow + '0';
		io_write(w, &c, 1);
		n %= pow;
		pow /= 10;
	}
}

void	io_write_one(t_write *out, char c)
{
	io_write(out, &c, 1);
}

void	io_write_int(t_write *out, uintmax_t nb, size_t nb_bytes)
{
	while (nb_bytes != 0)
	{
		nb_bytes--;
		io_write_one(out, (nb >> (nb_bytes * 8)) & 0xFF);
	}
}
