/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_vm.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbecerri <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/06 01:40:50 by fbecerri          #+#    #+#             */
/*   Updated: 2019/05/08 16:44:28 by prastoin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

bool		ft_winner(t_champ champ[MAX_PLAYERS], t_vm *vm)
{
	ssize_t		max;
	size_t		i;
	size_t		winner;

	i = 0;
	max = -1;
	while (i < MAX_PLAYERS)
	{
		if (max <= (ssize_t)champ[i].last_cycle_live && vm->champ[i].fd)
		{
			max = champ[i].last_cycle_live;
			winner = i;
		}
		i++;
	}
	if (vm->flags.verbose)
		ft_putf_fd(vm->v_fd, "Contestant %U, \"%s\", has won !\n", winner + 1,
				champ[winner].name);
	else
		ft_putf("%s a gagne avec le num %d\n", champ[winner].name, winner + 1);
	return (false);
}

bool		cycle_decremente_die(t_vm *vm)
{
	if (vm->nbr_live >= NBR_LIVE || vm->check + 1 == MAX_CHECKS)
	{
		vm->cycle_to_die -= CYCLE_DELTA;
		if (vm->flags.verbose)
			ft_putf_fd(vm->v_fd, "Cycle to die is now %d\n", vm->cycle_to_die);
		vm->check = 0;
	}
	else
		vm->check++;
	if (vm->cycle_to_die <= 0)
	{
		return (ft_winner(vm->champ, vm));
	}
	vm->nbr_live = 0;
	return (true);
}

void		kill_player(t_vm *vm)
{
	size_t i;

	i = 0;
	while (i < MAX_PLAYERS)
	{
		if (vm->said_live[i] == true)
			vm->said_live[i] = false;
		else
			vm->live[i] = false;
		i++;
	}
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
			if (vm->flags.verbose)
				ft_putf_fd(vm->v_fd,
						"Process %d hasn't lived for %d cycles (CTD %d)\n",
						i + 1, vm->cycle
						- vm->vec->processes[i].last_cycle_live,
						vm->cycle_to_die);
			vm->vec->processes[i].is_alive = false;
		}
		if (vm->vec->processes[i].is_alive == false)
			dead++;
		if (dead == vm->vec->len)
			return (ft_winner(vm->champ, vm));
		i--;
	}
	return (true);
}

bool		vm_cycle_to_die(t_vm *vm)
{
	if (vm->i_to_die >= vm->cycle_to_die)
	{
		kill_player(vm);
		if (!kill_process(vm, 0))
			return (false);
		if (!cycle_decremente_die(vm))
			return (false);
		vm->i_to_die = 0;
	}
	vm->i_to_die++;
	vm->cycle++;
	if (vm->flags.verbose)
		ft_putf_fd(vm->v_fd, "It is now cycle %d\n", vm->cycle);
	return (true);
}
