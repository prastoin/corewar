/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_vm.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbecerri <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/06 01:40:50 by fbecerri          #+#    #+#             */
/*   Updated: 2019/05/21 14:15:37 by dde-jesu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

static bool	find_winner(t_vm *vm)
{
	ssize_t		max;
	size_t		i;
	size_t		winner;

	i = 0;
	max = -1;
	while (i < MAX_PLAYERS)
	{
		if (max <= (ssize_t)vm->champ[i].last_cycle_live && vm->champ[i].fd)
		{
			max = vm->champ[i].last_cycle_live;
			winner = i;
		}
		i++;
	}
	hook_win(vm, winner);
	return (false);
}

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
	if (vm->cycle_to_die <= 0)
	{
		return (find_winner(vm));
	}
	vm->nbr_live = 0;
	return (true);
}

bool		kill_process(t_vm *vm, size_t dead)
{
	ssize_t i;

	i = vm->vec->len - 1;
	while (i >= 0)
	{
		if (vm->vec->processes[i].said_live == true)
			vm->vec->processes[i].said_live = false;
		else if (vm->vec->processes[i].is_alive == true)
		{
			vm->vec->processes[i].is_alive = false;
			hook_process_die(vm, vm->vec->processes + i);
		}
		if (vm->vec->processes[i].is_alive == false)
			dead++;
		if (dead == vm->vec->len)
			return (find_winner(vm));
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
