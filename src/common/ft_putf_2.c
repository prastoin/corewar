/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putf_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/26 15:39:21 by prastoin          #+#    #+#             */
/*   Updated: 2019/05/04 13:27:59 by prastoin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_string.h"
#include <unistd.h>

int	ft_putf(char *fmt, ...)
{
	va_list	args;
	size_t	padd;

	padd = 0;
	va_start(args, fmt);
	ft_putf_va(STDOUT_FILENO, fmt, args, padd);
	va_end(args);
	return (0);
}

int		ft_putf_fd(int fd, char *fmt, ...)
{
	va_list	args;
	size_t	padd;

	padd = 0;
	va_start(args, fmt);
	ft_putf_va(fd, fmt, args, padd);
	va_end(args);
	return (0);
}

/*
int main(int argc, const char *argv[])
{
	ft_putf_fd(1, "test1 = %2x, test2 = %4x\n", 454, 785);
	dprintf(1, "test1 = %2x, test2 = %4x", 454, 785);
	return 0;
}*/
