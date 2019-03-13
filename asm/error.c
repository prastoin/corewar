/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/13 17:54:07 by prastoin          #+#    #+#             */
/*   Updated: 2019/03/13 18:03:36 by prastoin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_string.h"

void		ft_putstr_fd(size_t fd, char *str)
{
	size_t	i;

	i = -1;
	while (str[++i])
		write(fd, str + i, 1);
}

void	ex_error(char *str)
{
	write(2, CSI_RED, (sizeof(CSI_RED) - 1));
	ft_putstr_fd(2, str);
	write(2, CSI_RESET, (sizeof(CSI_RESET) - 1));
	exit(0);
}
