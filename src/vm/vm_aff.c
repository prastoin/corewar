/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vm_aff.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbecerri <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/06 01:48:59 by fbecerri          #+#    #+#             */
/*   Updated: 2019/06/05 10:02:38 by dde-jesu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

void		dump_mem(t_vm *vm)
{
	size_t i;

	i = 0;
	if (!vm->flags.verbose)
	{
		while (i < MEM_SIZE)
		{
			if (i % 32 == 0 && i != 0)
				ft_putf_fd(STDOUT_FILENO, "\n");
			if (i % 32 == 0)
				ft_putf_fd(STDOUT_FILENO, "0x%4x : ", i);
			ft_putf_fd(STDOUT_FILENO, "%2x ", vm->mem[i++]);
		}
		ft_putf_fd(STDOUT_FILENO, "\n");
		return ;
	}
	while (i < MEM_SIZE)
	{
		if (i % 64 == 0 && i != 0)
			io_putf(&vm->v, "\n");
		if (i % 64 == 0)
			io_putf(&vm->v, "0x%4x : ", i);
		io_putf(&vm->v, "%2x ", vm->mem[i++]);
	}
	io_putf(&vm->v, "\n");
}

void		affstart_verbose(t_vm *vm)
{
	size_t		i;
	t_champ		*champ;

	i = 0;
	io_putf(&vm->v, "Introducing contestants...\n");
	while (i < MAX_PLAYERS)
	{
		if (vm->champ[i].fd)
		{
			champ = vm->champ + i;
			io_putf(&vm->v,
				"* Player %U, weighing %U bytes, \"%s\" (\"%s\") !\n",
				i + 1, champ->size, champ->name, champ->comment);
		}
		i++;
	}
}

t_vm		init_vm(void)
{
	return ((t_vm) {
		.cycle_to_die = CYCLE_TO_DIE,
		.flags = {
			.dump_c = -1
		}
	});
}
