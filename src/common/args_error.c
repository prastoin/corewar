/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_error.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/26 12:36:57 by prastoin          #+#    #+#             */
/*   Updated: 2019/04/26 15:35:42 by prastoin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_string.h"
#include "common.h"

int	show_err(int err, char *name, char *option, size_t len)
{
	if (!err)
		return (0);
	if (err == UNKNOWN_OPTION)
	{
		write(2, name, ft_strlen(name));
		write(2, ": illegal option -- ", 20);
		write(2, option, len);
		write(2, "\n", 1);
	}
	else if (err == NO_ARG)
	{
		write(2, name, ft_strlen(name));
		write(2, ": argument required -- ", 23);
		write(2, option, len);
		write(2, "\n", 1);
	}
	return (1);
}
