/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paul.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/03 11:18:42 by prastoin          #+#    #+#             */
/*   Updated: 2019/04/05 15:30:20 by prastoin         ###   ########.fr       */
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

	i = 0;
	while (i < 3)
	{
		type = (ocp >> ((3 - i) * 2)) & 0b11;
		if (type == 0b01)
			(size) += 1;
		else if (type == 0b10)
			(size) += g_ops[opcode].params[i] & PARAM_INDEX ? 2 : 4;
		else if (type == 0b11)
			(size) += 2;
		i++;
	}
	return(size);
}

bool	carry_up(t_process *process, uint8_t ocp, int opcode)
{
	size_t decale;

	process->carry = 1;
	decale = get_decale(ocp, opcode);
	inc_process_off_mod(process, decale, false);
	return (true);
}

bool	carry_down(t_process *process)
{
	process->carry = -1;
	inc_process_off_mod(process, 1, false);
	return (false);
}

bool	invalid(t_process *process)
{
	inc_process_off_mod(process, 1, false);
	return (false);
}

bool	valid(t_process *process, uint8_t ocp, int opcode)
{
	size_t decale;

	decale = get_decale(ocp, opcode);
	inc_process_off_mod(process, decale, false);
	return (true);
}

void	inc_process_off_mod(t_process *process, size_t size, bool mod)
{
	if (mod == false)
		process->offset = (process->offset + size) % MEM_SIZE;
	else
	{
		process->offset += size % IDX_MOD;
		process->offset = process->offset % MEM_SIZE;
	}
}
