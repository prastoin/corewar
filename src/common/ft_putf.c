/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putf_fd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/08 15:31:33 by dde-jesu          #+#    #+#             */
/*   Updated: 2019/04/02 13:56:28 by prastoin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "asm.h"
#include <stdarg.h>
#include "ft_string.h"

void		ft_putchar_fd(size_t fd, uint8_t c)
{
	write(fd, &c, 1);
}

void		ft_putstr_fd(char *str, size_t fd)
{
	size_t i;

	i = -1;
	while (str[++i])
		ft_putchar_fd(fd, str[i]);
}

char		*ft_strchr(const char *s, int c)
{
	size_t	i;
	size_t	max;

	i = 0;
	max = ft_strlen(s);
	while (s[i] != c && s[i])
		i++;
	if (i == max && s[i] != c)
		return (0);
	else
		return ((char *)(s + i));
}

void		ft_putnbr_fd(size_t fd, intmax_t nb)
{
	if (nb < 0)
	{
		ft_putchar_fd(fd, '-');
		nb = -nb;
	}
	if (nb > 9)
	{
		ft_putnbr_fd(fd, nb / 10);
		ft_putnbr_fd(fd, nb % 10);
	}
	else
		ft_putchar_fd(fd, nb + '0');
}

void	ft_putf_va(int fd, char *fmt, va_list args)
{
	char	*l_fmt;

	l_fmt = fmt;
	while ((fmt = ft_strchr(fmt, '%')) && *++fmt)
	{
		write(fd, l_fmt, fmt - l_fmt - 1);
		if (*fmt == '%')
			ft_putchar_fd('%', fd);
		else if (*fmt == 'd')
			ft_putnbr_fd(va_arg(args, int), fd);
		else if (*fmt == 's')
			ft_putstr_fd(va_arg(args, char *), fd);
		else if (*fmt == 'c')
			ft_putchar_fd((char)va_arg(args, int), fd);
		else if (*fmt == 'z')
			ft_putchar_fd((char)va_arg(args, size_t), fd);
		l_fmt = ++fmt;
	}
	ft_putstr_fd(l_fmt, fd);
}

void	ft_putf(char *fmt, ...)
{
	va_list	args;

	va_start(args, fmt);
	ft_putf_va(STDOUT_FILENO, fmt, args);
	va_end(args);
}

void	ft_putf_fd(int fd, char *fmt, ...)
{
	va_list	args;

	va_start(args, fmt);
	ft_putf_va(fd, fmt, args);
	va_end(args);
}
