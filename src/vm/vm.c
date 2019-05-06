/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vm.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/28 09:01:43 by prastoin          #+#    #+#             */
/*   Updated: 2019/05/06 11:05:14 by dde-jesu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"
#include "common.h"
#include "ft_string.h"
#include <fcntl.h>
#include <limits.h>
#define FLAG_N_MSG "Choose the number for a player"
#define FLAG_D_MSG "Dump memory on 0 at N cycle"
#define FLAG_R_MSG "On sait pas on parse"
#define FLAG_B_MSG "Affichage binnaire"
#define FLAG_C_MSG "Affichage Ncurse"
#define FLAG_V_MSG "Enabled Verbose mode"

bool		insert_player(t_vm *vm, char *name, int n, bool flag)
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
		vm->live[n - 1] = true;
	}
	return (true);
}

static bool	is_empty(char *players[MAX_PLAYERS + 1])
{
	size_t	i;

	i = 0;
	while (i < MAX_PLAYERS)
	{
		if (!players[i])
			return (false);
		i++;
	}
	return (true);
}

int			main_split(char *players[MAX_PLAYERS + 1], char *argv[], int argc,
		t_vm vm)
{
	size_t i;

	i = 0;
	while (i < MAX_PLAYERS)
	{
		if (!insert_player(&vm, players[i], i, true))
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

t_vm		init_vm(void)
{
	t_vm vm;

	return (
		vm = (t_vm) {
			.cycle_to_die = CYCLE_TO_DIE,
			.flags = {
				.dump_c = -1
			}
	});
}

int			main(int argc, char *argv[])
{
	t_vm		vm;
	ssize_t		ret;
	char		*players[MAX_PLAYERS + 1];
	const t_arg args[] = {
		{ARG_PLAYERS, 'n', "number", &players, FLAG_N_MSG},
		{ARG_INT, 'd', "dump", &vm.flags.dump_c, FLAG_D_MSG},
		{ARG_INT, 'r', "run", &vm.flags.run_c, FLAG_R_MSG},
		{ARG_BOOLEAN, 'b', "bin_aff", &vm.flags.bin_o, FLAG_B_MSG},
		{ARG_BOOLEAN, 'c', "ncurse_aff", &vm.flags.ncurse_o, FLAG_C_MSG},
		{ARG_BOOLEAN, 'v', "verbose", &vm.flags.verbose, FLAG_V_MSG},
		{ARG_END, 0, 0, 0, 0}
	};

	vm = init_vm();
	ft_memset(players, 0, sizeof(players));
	if ((ret = parse_args(args, argc, argv)) < 0
			|| (is_empty(players) && argc == ret))
		return (args_usage(args, argv[0], "source_file", "Launch corewar vm"));
	if (vm.flags.verbose == true)
		vm.v_fd = open("verbose", O_RDWR | O_CREAT | O_TRUNC,
				S_IRUSR | S_IWUSR);
	return (main_split(players, argv + ret, argc - ret, vm));
}
