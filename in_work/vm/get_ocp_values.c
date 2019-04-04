/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_ocp_values.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/03 15:21:34 by prastoin          #+#    #+#             */
/*   Updated: 2019/04/03 16:53:16 by prastoin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

bool		ft_check_ocp(uint8_t ocp, uint8_t opcode)
{
	size_t i;
	size_t type;

	i = 0;
	while (g_ops[opcode].params[i])
	{
		type = (ocp >> (3 - i) * 2) & 0b11;
		if (!(type & g_ops[opcode].params[i]))
			return (false);
		i++;
	}
	return (true);
}

ssize_t		ft_get_value(ssize_t nbr, uint8_t type, t_process *process, t_vm *vm)
{
	ssize_t value;
	uint8_t str[8];

	process->success = true;
	if (type == OCP_REG)
	{
		if (nbr >= 16)
			process->success = false;
		else
			value = process->registre[nbr];
	}
	else if (type == OCP_DIR)
		value = nbr;
	else if (type == OCP_IND)
		value = (int16_t)mem_read(vm->mem, str, (process->offset + nbr) % MEM_SIZE, 2);
	else
		process->success = false;
	return (value);
}

ssize_t		ft_get_value_mod(ssize_t nbr, uint8_t type, t_process *process, t_vm *vm)
{
	ssize_t value;
	uint8_t str[8];

	process->success = true;
	if (type == OCP_REG)
	{
		if (nbr >= 16)
			process->success = false;
		else
			value = (int16_t)process->registre[nbr];
	}
	else if (type == OCP_DIR)
		value = nbr;
	else if (type == OCP_IND)
		value = (int16_t)mem_read(vm->mem, str, (process->offset + nbr % IDX_MOD) % MEM_SIZE, 2);
	else
		process->success = false;
	return (value);
}
