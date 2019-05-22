/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   play.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/04 10:13:41 by prastoin          #+#    #+#             */
/*   Updated: 2019/05/22 23:15:22 by prastoin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

void		exec_process(t_vm *vm)
{
	ssize_t		i;
	t_process	*process;

	i = vm->vec->len - 1;
	while (i >= 0)
	{
		process = vm->vec->processes + i;
		if (process->is_alive)
		{
			if (process->cycle_to_do == 0)
			{
				vm->c_pc = i + 1;
				if (!process->has_read)
					read_opcode(vm, process);
				else if (process->has_read)
				{
					ft_pass(vm, process);
					process = vm->vec->processes + i;
				}
			}
			if (process->cycle_to_do > 0)
				process->cycle_to_do--;
		}
		i--;
	}
}

bool		david_needs_to_work(t_vm *vm)
{
	while (true)
	{
		exec_process(vm);
		if (!vm_cycle_to_die(vm))
			return (true);
		if (hook_cycle_end(vm))
			break ;
	}
	return (false);
}
