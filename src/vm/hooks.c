/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/20 15:37:18 by prastoin          #+#    #+#             */
/*   Updated: 2019/05/20 16:20:17 by prastoin         ###   ########.fr       */
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

bool	hook_cycle_end()
{
	return (false);
}

void	hook_process_read_opcode(t_process *process, uint8_t opcode)
{
}

void	hook_process_memory_write(t_process *process, size_t offset, size_t size)
{
}

void	hook_cycle_to_die(size_t value)
{
}
