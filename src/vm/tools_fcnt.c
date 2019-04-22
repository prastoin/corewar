/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paul.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/03 11:18:42 by prastoin          #+#    #+#             */
/*   Updated: 2019/04/22 17:17:20 by prastoin         ###   ########.fr       */
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
	while (i < 3)
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
	printf("\n\033[32;01mand go through %zu bytes\033[0m\n", size);
	return (size);
}

bool	carry_up(t_process *process, uint8_t ocp, int opcode)
{
	size_t decale;

	process->carry = 1;
	decale = get_decale(ocp, opcode);
	process->offset = (process->offset + decale) % MEM_SIZE;
	return (true);
}

bool	carry_down(t_process *process, uint8_t ocp, int opcode)
{
	size_t decale;

	decale = get_decale(ocp, opcode);
	process->carry = 0;
	printf("\n\033[31m and has failed\033[0m\n");
	process->offset = (process->offset + 1) % MEM_SIZE;
	return (false);
}

bool	invalid(t_process *process, uint8_t ocp, int opcode)
{
	size_t decale;

	decale = get_decale(ocp, opcode);
	printf("\n\033[31m and has failed\033[0m\n");
	process->offset = (process->offset + decale) % MEM_SIZE;
	return (false);
}

bool	valid(t_process *process, uint8_t ocp, int opcode)
{
	size_t decale;

	decale = get_decale(ocp, opcode);
	process->offset = (process->offset + decale) % MEM_SIZE;
	return (true);
}
