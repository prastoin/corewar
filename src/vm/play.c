/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   play.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/04 10:13:41 by prastoin          #+#    #+#             */
/*   Updated: 2019/05/06 10:33:55 by fbecerri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"
#include "ft_string.h"

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
			if (process->cycle_to_do == 0 && process->has_read)
			{
				vm->c_pc = i + 1;
				ft_pass(vm, process);
				process = vm->vec->processes + i;
			}
			if (process->cycle_to_do > 0)
				process->cycle_to_do--;
		}
		i--;
	}
}

void		david_needs_to_work(t_vm *vm)
{
	ssize_t		i;
	t_process	*process;

	vm->continu = true;
	while (vm->continu)
	{
		exec_process(vm);
		i = 0;
		while ((size_t)i < vm->vec->len)
		{
			vm->c_pc = i + 1;
			process = vm->vec->processes + i;
			if (process->cycle_to_do == 0 && !process->has_read)
				read_opcode(vm, process);
			i++;
		}
		if (vm->cycle == (uintmax_t)vm->flags.dump_c)
			dump_mem(vm);
		vm->continu = vm_cycle_to_die(vm);
	}
	return ;
}
