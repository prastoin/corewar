/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   argv_management.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/27 14:51:06 by prastoin          #+#    #+#             */
/*   Updated: 2019/05/13 14:39:00 by fbecerri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common.h"
#include "ft_string.h"

static int	args_players(const t_arg *opt, char *value, char *argv[], int i[2])
{
	int		player;

	if (!(value = argv[++i[0]]))
		return (NO_ARG);
	player = ft_atoi(value);
	if (!(value = argv[++i[0]]))
		return (NO_ARG);
	if (player > 0 && player <= MAX_PLAYERS)
	{
		((char **)opt->value)[player - 1] = value;
		return (0);
	}
	else
		return (INVALID_VALUE);
	return (0);
}

int			get_value(char **c_arg, const t_arg *opt, char *argv[], int i[2])
{
	char	*arg;
	char	*value;

	arg = *c_arg + i[1];
	value = NULL;
	if (opt->type == ARG_BOOLEAN)
		*(bool *)opt->value = true;
	else if (opt->type == ARG_INT || opt->type == ARG_STR)
	{
		if (*arg == '=')
		{
			value = arg + 1;
			*c_arg += i[1] + ft_strlen(arg) - 1;
		}
		else if (!*arg)
			value = argv[++i[0]];
		if (!value)
			return (NO_ARG);
		(opt->type == ARG_INT) ? (void)(*(int *)opt->value = ft_atoi(value))
			: (void)(*(char **)opt->value = value);
	}
	else if (opt->type == ARG_PLAYERS)
		return (args_players(opt, value, argv, i));
	return (0);
}

bool		parse_long(const t_arg opt[], char **arg, char *argv[], size_t *i)
{
	size_t	len;
	char	*res;
	int		indexes[2];
	int		err;

	if ((res = ft_strchr(*arg, '=')))
		len = res - *arg;
	else
		len = ft_strlen(*arg);
	while (opt->type != ARG_END)
	{
		if (ft_strncmp((uint8_t*)opt->long_name, *arg, len) == 0)
		{
			indexes[0] = *i;
			indexes[1] = len;
			err = get_value(arg, opt, argv, indexes);
			*i = *indexes;
			return (show_err(err, argv[0], *arg, len));
		}
		opt++;
	}
	return (show_err(UNKNOWN_OPTION, argv[0], *arg, ft_strlen(*arg)));
}

bool		parse_short(const t_arg opt[], char **arg, char *argv[], size_t *i)
{
	int		indexes[2];
	int		err;

	while (opt->type != ARG_END)
	{
		if (opt->short_name == **arg)
		{
			indexes[0] = *i;
			indexes[1] = 1;
			err = get_value(arg, opt, argv, indexes);
			*i = *indexes;
			return (show_err(err, argv[0], *arg, 1));
		}
		opt++;
	}
	return (show_err(UNKNOWN_OPTION, argv[0], *arg, 1));
}

ssize_t		parse_args(const t_arg args[], int argc, char *argv[])
{
	size_t	i;
	size_t	err;
	char	*arg;

	i = 0;
	err = 0;
	while ((int)++i < argc)
	{
		arg = argv[i];
		if (arg[0] != '-' || !arg[1])
			return (err ? -1 : i);
		if (arg[1] != '-')
		{
			while (*++arg)
				err |= parse_short(args, &arg, argv, &i);
		}
		else
		{
			if (!arg[2])
				return (err ? -1 : i + 1);
			arg += 2;
			err |= parse_long(args, &arg, argv, &i);
		}
	}
	return (err ? -1 : i);
}
