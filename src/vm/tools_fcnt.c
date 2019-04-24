/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paul.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/03 11:18:42 by prastoin          #+#    #+#             */
/*   Updated: 2019/04/24 10:56:36 by prastoin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

#define REG_T 0b01
#define IND_T 0b11
#define DIR_T 0b10

size_t		get_decale(uint8_t ocp, int opcode)
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

void	verbose(size_t decale, t_process *process, uint8_t ocp, int opcode)
{
	size_t size;
	size_t i;
	size_t type;

	i = 0;
	size = g_ops[opcode].ocp ? 2 : 1;
	dprintf(g_fd, "ADV %d (0x%.4x -> 0x%.4x) ", decale, process->offset, (process->offset + decale) % MEM_SIZE);
	while (g_ops[opcode].params[i])
	{
		type = (ocp >> ((3 - i) * 2)) & 0b11;
		if (type == 0b01)
			size += 1;
		else if (type == 0b10)
			size += (g_ops[opcode].params[i] & PARAM_INDEX ? 2 : 4);
		else if (type == 0b11)
			size += 2;
		i++;
	}
	i = 0;
	while (i < size)
	{
		dprintf (g_fd, "%.2x ", g_vm->mem[(process->offset + i) % MEM_SIZE]);
		i++;
	}
	dprintf(g_fd, "\n");
}

bool	carry_up(t_process *process, uint8_t ocp, int opcode)
{
	size_t decale;

	process->carry = true;
	decale = get_decale(ocp, opcode);
	verbose(decale, process, ocp, opcode);
	process->offset = (process->offset + decale) % MEM_SIZE;
	return (true);
}

bool	carry_down(t_process *process, uint8_t ocp, int opcode)
{
	size_t decale;

	decale = get_decale(ocp, opcode);
	process->carry = false;
	verbose(decale, process, ocp, opcode);
	process->offset = (process->offset + decale) % MEM_SIZE;
	return (false);
}

bool	invalid(t_process *process, uint8_t ocp, int opcode)
{
	size_t decale;

	decale = get_decale(ocp, opcode);
	verbose(decale, process, ocp, opcode);
	process->offset = (process->offset + decale) % MEM_SIZE;
	return (false);
}

bool	valid(t_process *process, uint8_t ocp, int opcode)
{
	size_t decale;

	decale = get_decale(ocp, opcode);
	verbose(decale, process, ocp, opcode);
	process->offset = (process->offset + decale) % MEM_SIZE;
	return (true);
}
