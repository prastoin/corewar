/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paul.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/03 11:18:42 by prastoin          #+#    #+#             */
/*   Updated: 2019/05/22 23:16:22 by prastoin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

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
			size += g_ops[opcode].params[i] & Param_Index ? 2 : 4;
		else if (type == 0b11)
			size += 2;
		i++;
	}
	return (size);
}

bool	carry_up(t_vm *vm, t_process *process, uint8_t ocp, int opcode)
{
	size_t decale;

	process->carry = true;
	decale = get_decale(ocp, opcode);
	hook_process_adv(vm, process, decale);
	process->offset = (process->offset + decale) % MEM_SIZE;
	return (true);
}

bool	carry_down(t_vm *vm, t_process *process, uint8_t ocp, int opcode)
{
	size_t decale;

	decale = get_decale(ocp, opcode);
	process->carry = false;
	hook_process_adv(vm, process, decale);
	process->offset = (process->offset + decale) % MEM_SIZE;
	return (false);
}

bool	invalid(t_vm *vm, t_process *process, uint8_t ocp, int opcode)
{
	size_t decale;

	decale = get_decale(ocp, opcode);
	hook_process_adv(vm, process, decale);
	process->offset = (process->offset + decale) % MEM_SIZE;
	return (true);
}

bool	valid(t_vm *vm, t_process *process, uint8_t ocp, int opcode)
{
	size_t decale;

	decale = get_decale(ocp, opcode);
	hook_process_adv(vm, process, decale);
	process->offset = (process->offset + decale) % MEM_SIZE;
	return (true);
}
