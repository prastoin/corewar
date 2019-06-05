/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putf_fd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/08 15:31:33 by dde-jesu          #+#    #+#             */
/*   Updated: 2019/06/05 10:14:48 by prastoin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_string.h"

void			char_fd(t_write *out, uint8_t c, size_t padd)
{
	io_pad(out, ' ', padd - 1);
	io_write(out, &c, 1);
}

static void		str_fd(t_write *out, char *str, size_t padd)
{
	const size_t	len = ft_strlen(str);

	io_pad(out, ' ', padd - len);
	io_write(out, str, len);
}

void			str_hexa_fd(t_write *out, char *str, size_t padd)
{
	const size_t	len = ft_strlen(str);

	io_pad(out, '0', padd - len);
	io_write(out, str, len);
}

void			putnbr_fd(t_write *out, intmax_t nb)
{
	if (nb < 0)
	{
		char_fd(out, '-', 0);
		nb = -nb;
	}
	if (nb > 9)
	{
		putnbr_fd(out, nb / 10);
		putnbr_fd(out, nb % 10);
	}
	else
		char_fd(out, nb + '0', 0);
}

void			ft_putf_va(t_write *out, char *fmt, va_list args, size_t padd)
{
	char		*l_fmt;

	l_fmt = fmt;
	while ((fmt = ft_strchr(fmt, '%')) && *++fmt)
	{
		padd = 0;
		io_write(out, l_fmt, fmt - l_fmt - 1);
		if (*fmt >= '0' && *fmt <= '9')
		{
			padd = ft_atoi(fmt);
			fmt += nb_len(padd);
		}
		if (*fmt == '%')
			char_fd(out, '%', padd);
		else if (*fmt == 's')
			str_fd(out, va_arg(args, char *), padd);
		else if (*fmt == 'c')
			char_fd(out, (char)va_arg(args, int), padd);
		else if (*fmt == 'x' || *fmt == 'X')
			handle_hexa(out, args, padd, fmt);
		else
			handle_dlu(out, args, padd, fmt);
		l_fmt = ++fmt;
	}
	io_write(out, l_fmt, ft_strlen(l_fmt));
}
