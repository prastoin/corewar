/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   play.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/04 10:13:41 by prastoin          #+#    #+#             */
/*   Updated: 2019/06/04 15:28:47 by prastoin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

bool		exec_process(t_vm *vm)
{
	ssize_t		i;

	i = vm->vec->len;
	while (--i >= 0)
	{
		if (vm->vec->processes[i].is_alive)
		{
			if (vm->vec->processes[i].cycle_to_do == 0)
			{
				vm->c_pc = i + 1;
				if (!vm->vec->processes[i].has_read)
					read_opcode(vm, vm->vec->processes + i);
				else if (vm->vec->processes[i].has_read)
					if (!ft_pass(vm, vm->vec->processes + i))
						return (false);
			}
			if (vm->vec->processes[i].cycle_to_do > 0)
				vm->vec->processes[i].cycle_to_do--;
		}
	}
	return (true);
}

bool		david_needs_to_work(t_vm *vm)
{
	while (true)
	{
		if (!exec_process(vm))
			return (true);
		if (!vm_cycle_to_die(vm))
			return (true);
		if (hook_cycle_end(vm))
			break ;
	}
	return (false);
}
