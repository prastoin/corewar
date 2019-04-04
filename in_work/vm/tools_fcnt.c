/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paul.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/03 11:18:42 by prastoin          #+#    #+#             */
/*   Updated: 2019/04/04 16:46:51 by prastoin         ###   ########.fr       */
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
	process->offset += decale;
	return (true);
}

bool	carry_down(t_process *process)
{
	process->carry = -1;
	process->offset++;
	return (false);
}

bool	invalid(t_process *process)
{
	process->offset++;
	return (false);
}

bool	valid(t_process *process, uint8_t ocp, int opcode)
{
	size_t decale;

	decale = get_decale(ocp, opcode);
	process->offset += decale;
	return (true);
}
