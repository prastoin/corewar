/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putf_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/26 15:39:21 by prastoin          #+#    #+#             */
/*   Updated: 2019/04/26 16:08:57 by prastoin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_string.h"
#include <unistd.h>

int	ft_putf(char *fmt, ...)
{
	va_list	args;

	va_start(args, fmt);
	ft_putf_va(STDOUT_FILENO, fmt, args);
	va_end(args);
	return (0);
}

int		ft_putf_fd(int fd, char *fmt, ...)
{
	va_list	args;

	va_start(args, fmt);
	ft_putf_va(fd, fmt, args);
	va_end(args);
	return (0);
}
