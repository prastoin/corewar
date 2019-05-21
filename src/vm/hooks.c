/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/20 15:37:18 by prastoin          #+#    #+#             */
/*   Updated: 2019/05/21 14:39:29 by dde-jesu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

void	hook_process_adv(t_vm *vm, t_process *process, size_t diff)
{
	size_t	i;

	if (vm->flags.verbose && diff > 1)
	{
		ft_putf_fd(vm->v_fd, "ADV %d (0x%4X -> 0x%4X) ", diff,
				process->offset, ((process->offset + diff)));
		i = 0;
		while (i < diff)
		{
			ft_putf_fd(vm->v_fd, "%2x ",
					vm->mem[(process->offset + i) % MEM_SIZE]);
			i++;
		}
		ft_putf_fd(vm->v_fd, "\n");
	}
}

void	hook_process_jump(t_vm *vm, t_process *process, uint32_t param, size_t offset)
{
	if (vm->flags.verbose)
	{
		ft_putf_fd(vm->v_fd, "P %4d | zjmp %d %s\n", process - vm->vec->processes + 1,
			param, process->carry == true ? "OK" : "FAILED");
	}
}

void	hook_process_spawn(t_process *process, t_process *parent, size_t offset)
{
}

bool	hook_cycle_end(t_vm *vm)
{
	if (vm->cycle - 1 == (uintmax_t)vm->flags.dump_c)
	{
		dump_mem(vm);
		return (true);
	}
	if (vm->flags.verbose)
		ft_putf_fd(vm->v_fd, "It is now cycle %d\n", vm->cycle);
	return (false);
}

void	hook_process_read_opcode(t_process *process, uint8_t opcode)
{
}

void	hook_process_memory_write(t_process *process, size_t offset, size_t size)
{
}

void	hook_cycle_to_die(t_vm *vm, size_t value)
{
	if (vm->flags.verbose)
		ft_putf_fd(vm->v_fd, "Cycle to die is now %d\n", value);
}

void	hook_process_live(t_vm *vm, t_process *process, size_t player)
{
	if (player >= 1 && player <= MAX_PLAYERS)
	{
		if (vm->champ[player].fd)
		{
			if (!vm->flags.verbose)
				ft_putf_fd(1,
						"un processus dit que le joueur %d(%s) est en vie\n",
						player + 1, vm->champ[player].name);
		}
	}
}

void	hook_process_die(t_vm *vm, t_process *process)
{
	if (vm->flags.verbose)
		ft_putf_fd(vm->v_fd,
				"Process %d hasn't lived for %d cycles (CTD %d)\n",
				process - vm->vec->processes + 1, vm->cycle
				- process->last_cycle_live,
				vm->cycle_to_die);
}

void	hook_win(t_vm *vm, size_t winner)
{
	if (vm->flags.verbose)
		ft_putf_fd(vm->v_fd, "Contestant %U, \"%s\", has won !\n", winner + 1,
				vm->champ[winner].name);
	else
		ft_putf("%s a gagne avec le num %d\n", vm->champ[winner].name, winner + 1);
}
