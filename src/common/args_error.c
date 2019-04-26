/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_error.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/26 12:36:57 by prastoin          #+#    #+#             */
/*   Updated: 2019/04/26 16:54:07 by prastoin         ###   ########.fr       */
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

int		args_usage(const t_arg args[], char *name, char *usage, char *desc)
{
	ft_putf_fd(1, "Usage: %s %s\n%s\n\nOptions:\n", name, usage, desc);
	while (args->type != ARG_END)
	{
		if (args->short_name && args->long_name)
			ft_putf_fd(1, "  -%c, --%s\n", args->short_name,
					args->long_name);
		else if (args->short_name)
			ft_putf_fd(1, "  -%c\n", args->short_name);
		else if (args->long_name)
			ft_putf_fd(1, "     --%s\n", args->long_name);
		args++;
	}
	return (0);
}
