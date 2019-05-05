/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paul.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/03 11:18:42 by prastoin          #+#    #+#             */
/*   Updated: 2019/05/06 01:05:19 by fbecerri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

#define REG_T 0b01
#define IND_T 0b11
#define DIR_T 0b10

size_t	get_decale(uint8_t ocp, int opcode)
{
	size_t size;
	size_t type;
	size_t i;

	size = g_ops[opcode].ocp ? 2 : 1;
	i = 0;
	while (g_ops[opcode].params[i])
	{
		type = (ocp >> ((3 - i) * 2)) & 0b11;
		if (type == 0b01)
			size += 1;
		else if (type == 0b10)
			size += g_ops[opcode].params[i] & PARAM_INDEX ? 2 : 4;
		else if (type == 0b11)
			size += 2;
		i++;
	}
	return (size);
}

bool	carry_up(t_vm *vm, t_process *process, uint8_t ocp, int opcode)
{
	size_t decale;
	size_t i;

	i = 0;
	process->carry = true;
	decale = get_decale(ocp, opcode);
	if (vm->flags.verbose)
	{
		ft_putf_fd(vm->v_fd, "ADV %d (0x%4X -> 0x%4X) ", decale,
				process->offset, ((process->offset + decale)));
		while (i < decale)
		{
			ft_putf_fd(vm->v_fd, "%2x ", vm->mem[(process->offset + i)
					% MEM_SIZE]);
			i++;
		}
		ft_putf_fd(vm->v_fd, "\n");
	}
	process->offset = (process->offset + decale) % MEM_SIZE;
	return (true);
}

bool	carry_down(t_vm *vm, t_process *process, uint8_t ocp, int opcode)
{
	size_t decale;
	size_t i;

	i = 0;
	decale = get_decale(ocp, opcode);
	process->carry = false;
	if (vm->flags.verbose)
	{
		ft_putf_fd(vm->v_fd, "ADV %d (0x%4X -> 0x%4X) ", decale,
				process->offset, ((process->offset + decale)));
		while (i < decale)
		{
			ft_putf_fd(vm->v_fd, "%2x ", vm->mem[(process->offset + i)
					% MEM_SIZE]);
			i++;
		}
		ft_putf_fd(vm->v_fd, "\n");
	}
	process->offset = (process->offset + decale) % MEM_SIZE;
	return (false);
}

bool	invalid(t_vm *vm, t_process *process, uint8_t ocp, int opcode)
{
	size_t decale;
	size_t i;

	i = 0;
	decale = get_decale(ocp, opcode);
	if (vm->flags.verbose)
	{
		ft_putf_fd(vm->v_fd, "ADV %d (0x%4X -> 0x%4X) ", decale,
				process->offset, ((process->offset + decale)));
		while (i < decale)
		{
			ft_putf_fd(vm->v_fd, "%2x ",
					vm->mem[(process->offset + i) % MEM_SIZE]);
			i++;
		}
		ft_putf_fd(vm->v_fd, "\n");
	}
	if (vm->c_pc == 50)
		printf("offset %ld cycle %ld\n", process->offset, vm->cycle);
	process->offset = (process->offset + decale) % MEM_SIZE;
	if (vm->c_pc == 50)
		printf("offset %ld should have decale %ld\n", process->offset, decale);
	return (false);
}

bool	valid(t_vm *vm, t_process *process, uint8_t ocp, int opcode)
{
	size_t decale;
	size_t i;

	i = 0;
	decale = get_decale(ocp, opcode);
	if (vm->flags.verbose)
	{
		ft_putf_fd(vm->v_fd, "ADV %d (0x%4X -> 0x%4X) ", decale,
				process->offset, ((process->offset + decale)));
		while (i < decale)
		{
			ft_putf_fd(vm->v_fd, "%2x ",
					vm->mem[(process->offset + i) % MEM_SIZE]);
			i++;
		}
		dprintf(vm->v_fd, "\n");
	}
	if (vm->c_pc == 50)
		printf("offset %ld cycle %ld\n", process->offset, vm->cycle);
	process->offset = (process->offset + decale) % MEM_SIZE;
	if (vm->c_pc == 50)
		printf("offset %ld should have decale %ld\n", process->offset, decale);
	return (true);
}
