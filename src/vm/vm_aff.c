/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vm_aff.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbecerri <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/06 01:48:59 by fbecerri          #+#    #+#             */
/*   Updated: 2019/06/04 09:22:46 by prastoin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

void		dump_mem(t_vm *vm)
{
	size_t i;
	size_t fd;
	size_t mod;

	if (!vm->flags.verbose)
	{
		mod = 32;
		fd = 1;
	}
	else
	{
		fd = vm->v_fd;
		mod = 64;
	}
	i = 0;
	while (i < MEM_SIZE)
	{
		if (i % mod == 0 && i != 0)
			ft_putf_fd(fd, "\n");
		if (i % mod == 0)
			ft_putf_fd(fd, "0x%4x : ", i);
		ft_putf_fd(fd, "%2x ", vm->mem[i]);
		i++;
	}
	ft_putf_fd(fd, "\n");
}

void		affstart_verbose(t_vm vm)
{
	size_t		i;
	t_champ		champ;

	i = 0;
	ft_putf_fd(vm.v_fd, "Introducing contestants...\n");
	while (i < MAX_PLAYERS)
	{
		if (vm.champ[i].fd)
		{
			champ = vm.champ[i];
			ft_putf_fd(vm.v_fd,
				"* Player %U, weighing %U bytes, \"%s\" (\"%s\") !\n",
				i + 1, champ.size, champ.name, champ.comment);
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
