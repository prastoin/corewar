/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/20 15:37:18 by prastoin          #+#    #+#             */
/*   Updated: 2019/06/05 12:10:27 by prastoin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

void	hook_cycle_to_die(t_vm *vm, size_t value)
{
	if (vm->flags.verbose)
		io_putf(&vm->v, "Cycle to die is now %d\n", value);
}

void	hook_process_live(t_vm *vm, t_process *process, size_t player)
{
	(void)process;
	if (player >= 1 && player <= MAX_PLAYERS)
	{
		if (vm->champ[player - 1].fd)
		{
			if (!vm->flags.verbose)
				ft_putf_fd(STDOUT_FILENO,
					"un processus dit que le joueur %d(%s) est en vie\n",
					player, vm->champ[player - 1].name);
		}
	}
}

void	hook_process_die(t_vm *vm, t_process *process)
{
	if (vm->flags.verbose)
		io_putf(&vm->v,
			"Process %d hasn't lived for %d cycles (CTD %d)\n",
			process - vm->vec->processes + 1, vm->cycle
			- process->last_cycle_live,
			vm->cycle_to_die);
}

void	hook_process_memory_write(t_process *process, size_t offset,
		size_t size)
{
	(void)process;
	(void)offset;
	(void)size;
}

void	hook_win(t_vm *vm, size_t winner)
{
	if (vm->flags.verbose)
		io_putf(&vm->v, "Contestant %U, \"%s\", has won !\n", winner + 1,
			vm->champ[winner].name);
	else
		ft_putf_fd(STDOUT_FILENO,
			"le joueur %d(%s) a gagne\n", winner + 1,
			vm->champ[winner].name);
}
