/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_vm.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbecerri <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/06 01:40:50 by fbecerri          #+#    #+#             */
/*   Updated: 2019/06/05 10:00:15 by dde-jesu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

bool		cycle_decremente_die(t_vm *vm)
{
	if (vm->nbr_live >= NBR_LIVE || vm->check + 1 == MAX_CHECKS)
	{
		vm->cycle_to_die -= CYCLE_DELTA;
		hook_cycle_to_die(vm, vm->cycle_to_die);
		vm->check = 0;
	}
	else
		vm->check++;
	vm->nbr_live = 0;
	return (true);
}

bool		kill_process(t_vm *vm, size_t dead)
{
	ssize_t i;

	i = vm->vec->len - 1;
	while (i >= 0)
	{
		if (vm->vec->processes[i].said_live && vm->cycle_to_die > 0)
			vm->vec->processes[i].said_live = false;
		else if (vm->vec->processes[i].is_alive)
		{
			vm->vec->processes[i].is_alive = false;
			hook_process_die(vm, vm->vec->processes + i);
		}
		if (!vm->vec->processes[i].is_alive)
			dead++;
		if (dead == vm->vec->len)
		{
			if (vm->cycle_to_die > 0)
				cycle_decremente_die(vm);
			hook_win(vm, vm->last_champ_alive);
			return (false);
		}
		i--;
	}
	return (true);
}

bool		vm_cycle_to_die(t_vm *vm)
{
	if (vm->i_to_die >= vm->cycle_to_die)
	{
		if (!kill_process(vm, 0))
			return (false);
		if (!cycle_decremente_die(vm))
			return (false);
		vm->i_to_die = 0;
	}
	vm->i_to_die++;
	vm->cycle++;
	return (true);
}
