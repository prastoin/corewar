/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vm.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/28 09:01:43 by prastoin          #+#    #+#             */
/*   Updated: 2019/04/23 13:58:21 by prastoin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"
#include "vm.h"
#include "ft_string.h"

#define UNKNOWN_OPTION (-1)
#define NO_ARG (-2)


static int	show_err(int err, char *name, char *option, size_t len	)
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

static int	get_value(char **c_arg, const t_arg *opt, char *argv[], int i[2])
{
	char	*arg;
	char	*value;

	arg = *c_arg + i[1]; // ??
	printf("--%s--\n", arg);
	if (opt->type == ARG_BOOLEAN)
		*(bool *)opt->value = true;
	else if (opt->type == ARG_INT)
	{
		value = NULL;
		if (*arg == '=')
		{
			value = arg + 1;
			*c_arg += i[1] + ft_strlen(arg) - 1;
		}
		else if (!*arg)
			value = argv[++i[0]];
		if (!value)
			return (NO_ARG);
		*(int *)opt->value = atoi(value); //TODO ft_atoi
	}
	else if (opt->type == ARG_STRUCT)
	{
		value = NULL;
		if (!*arg)
			value = argv[++i[0]];
		else
			return(UNKNOWN_OPTION);
		if (!value)
			return (NO_ARG);
		*((t_datan *)opt->value) = (t_datan) {
				.num = atoi(value),
				.name = argv[++i[0]]
		};
		if (((t_datan *)opt->value)->name == NULL)
			return (NO_ARG);
		//			*(t_arg *)opt->value; //TODO fill struct
	};
	return (0);
}

bool	parse_long(const t_arg opt[], char **arg, char *argv[], size_t *i)
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
			return (show_err(err, argv[0], *arg - len, len));
		}
		opt++;
	}
	return (show_err(UNKNOWN_OPTION, argv[0], *arg, ft_strlen(*arg)));
}

bool	parse_short(const t_arg opt[], char **arg, char *argv[], size_t *i)
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
			*i = *indexes; // === i = indexes[0] ?
			return (show_err(err, argv[0], *arg - 1, 1));
		}
		opt++;
	}
	return (show_err(UNKNOWN_OPTION, argv[0], *arg, 1));
}

bool		ft_check_is_struct(t_vm *vm, char *name, int n, bool flag) //parse arg after flags
{
	int			fd;
	static bool	player[MAX_PLAYERS];

	if (name)
	{
		if ((n - 1) >= MAX_PLAYERS || ft_strlen(name) > PATH_MAX)
			return (false);
		if ((fd = open(name, O_RDONLY)) < 0)
			return (false);
		if (!flag)
			while (n <= MAX_PLAYERS && player[n - 1] == true)
				n++;
		if (n > MAX_PLAYERS || n <= 0)
			return (false);
		if (player[n - 1] == true)
			return (false);
		else
			player[n - 1] = true;
		vm->champ[n - 1].fd = fd;
		vm->nbr_champ++;
	}
	printf("\n\nbool_Player[MAX_PLAYERS]\n%d\n", player[0]);
	printf("%d\n", player[1]);
	printf("%d\n", player[2]);
	printf("%d\n", player[3]);

	return (true);
}

ssize_t		parse_args(const t_arg args[], int argc, char *argv[], t_vm *vm)
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
		if (((t_datan *)args[0].value)->name && ((t_datan *)args[0].value)->num > 0 && ((t_datan *)args[0].value)->num <= MAX_PLAYERS)
			if (!(ft_check_is_struct(vm, ((t_datan *)args[0].value)->name, ((t_datan *)args[0].value)->num, true)))
				return (err ? -1 : i);
	}
	return (err ? -1 : i);
}

int main(int argc, char *argv[])
{
	t_vm		vm;
	ssize_t		ret;
	size_t		ok_champ;

	g_fd = open("verbose", O_RDWR | O_CREAT | O_TRUNC);
	ok_champ = 1;
	vm = (t_vm) {
		.cycle_to_die = CYCLE_TO_DIE,
		.flags = {
			.dump_c = -1
		}
	};
	const t_arg args[] = {
		{ARG_STRUCT, 'n', "number", &vm.flags.num, "Choose the number for a player"},
		{ARG_INT, 'd', "dump", &vm.flags.dump_c, "On sait pas on parse"},
		{ARG_INT, 'r', "run", &vm.flags.run_c, "On sait pas on parse"},
		{ARG_BOOLEAN, 'b', "bin_aff", &vm.flags.bin_o, "Affichage binnaire"},
		{ARG_BOOLEAN, 'c', "ncurse_aff", &vm.flags.ncurse_o, "Affichage Ncurse"},
		{ARG_END, 0, 0, 0, 0}
	};
	ret = -1;
	while (++ret < MAX_PLAYERS)
		vm.live[ret] = true;
	if ((ret = parse_args(args, argc, argv, &vm)) < 0)
		return (0);
	while (ret < argc)
	{
		printf("%s\n", argv[ret]);
		if (!ft_check_is_struct(&vm, argv[ret], ok_champ, false)) // n a 1 wtf ??
			return (0);
		else
			ok_champ++;
		ret++;
	}
//	printf("champ = %zu\n", vm.nbr_champ);
	if (vm.nbr_champ > 0 && vm.nbr_champ <= MAX_PLAYERS)
		ft_play(vm);
	return (0);
}
