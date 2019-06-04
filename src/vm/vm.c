/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vm.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/28 09:01:43 by prastoin          #+#    #+#             */
/*   Updated: 2019/06/04 09:24:25 by prastoin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"
#include "ft_string.h"
#include <fcntl.h>
#include <limits.h>

bool			insert_player(t_vm *vm, char *name, int n, bool flag)
{
	int			fd;
	static bool	player[MAX_PLAYERS];

	if (name)
	{
		if (ft_strlen(name) > PATH_MAX)
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
	return (true);
}

static bool		is_empty(char *players[MAX_PLAYERS + 1])
{
	size_t	i;

	i = 0;
	while (i < MAX_PLAYERS)
	{
		if (players[i])
			return (false);
		i++;
	}
	return (true);
}

int				main_split(char *players[MAX_PLAYERS + 1], char *argv[],
		int argc, t_vm vm)
{
	size_t i;

	i = 0;
	while (i < MAX_PLAYERS)
	{
		if (!insert_player(&vm, players[i], i + 1, true))
			return (ft_putf("Error in input files\n"));
		i++;
	}
	i = 0;
	while (i < (size_t)argc)
	{
		if (!insert_player(&vm, argv[i], i + 1, false))
			return (ft_putf("Error in input files\n"));
		i++;
	}
	if (vm.nbr_champ > 0 && vm.nbr_champ <= MAX_PLAYERS)
		ft_play(vm);
	return (0);
}

static t_arg	*create_args(char *players[MAX_PLAYERS], t_vm *vm)
{
	static t_arg args[4];

	args[0] = (t_arg) {
		Arg_Players, 'n', "number", &players, FLAG_N_MSG
	};
	args[1] = (t_arg) {
		Arg_Int, 'd', "dump", &vm->flags.dump_c, FLAG_D_MSG
	};
	args[2] = (t_arg) {
		Arg_Boolean, 'v', "verbose", &vm->flags.verbose, FLAG_V_MSG
	};
	args[3] = (t_arg) {
		.type = Arg_End
	};
	return (args);
}

int				main(int argc, char *argv[])
{
	t_vm		vm;
	ssize_t		ret;
	char		*players[MAX_PLAYERS];
	t_arg		*args;

	vm = init_vm();
	ft_memset(players, 0, sizeof(players));
	args = create_args(players, &vm);
	if ((ret = parse_args(args, argc, argv)) < 0
		|| (is_empty(players) && argc == ret))
		return (args_usage(args, argv[0], "source_file", "Launch corewar vm"));
	if (vm.flags.verbose == true)
		vm.v_fd = open("verbose", O_RDWR | O_CREAT | O_TRUNC,
				S_IRUSR | S_IWUSR);
	return (main_split(players, argv + ret, argc - ret, vm));
}
