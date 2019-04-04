/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fred.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbecerri <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/03 11:19:49 by fbecerri          #+#    #+#             */
/*   Updated: 2019/04/03 15:57:16 by prastoin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"
#include "op.h"

bool		sub(t_vm *game, t_process *process, size_t *param, uint8_t ocp)
{
	if (process->cycle_to_do < 10)
		return (false);
	return (true);
}

bool		st(t_vm *game, t_process *process, size_t *param, uint8_t ocp)
{
	int8_t	value;

	if (process->cycle_to_do < 5)
		return (false);
	value = (ocp >> 6) & 0b11;
	if (param[0] > 16 || (value == 1 && param[1] > 16))
		return (false);
	if (value == 1)
		process->registre[param[1]] = process->registre[param[0]];
	else if (value == 3)
		mem_write_int(mem, process->registre[param[0]], REG_SIZE, (process->offset + (param[1] % IDX_MOD)) % MEM_SIZE);
	else
		return (false);
	return (true);
}

bool		or(t_vm *game, t_process *process, size_t *param, uint8_t ocp)
{
	int8_t	value;
	int32_t		op1;
	int32_t		op2;

	if (process->cycle_to_do < 6)
		return (false);
	value = (ocp >> 2) & 0b11;
	if (value != OCP_REG || (value == OCP_REG && param[2] > 16))
		return (false);
	value = (ocp >> 4) & 0b11;
	if (((ocp >> 4) & 0b11) == OCP_REG)
	{
		if (param[1] > 16)
			return (false);
		else
			op2 = process->registre[param[1]];
	}
	else if (value == OCP_IND || value == OCP_DIR)
		op2 = param[1];
	else
		return (false);
	value = (ocp >> 6) & 0b11;
	if (value == OCP_REG)
	{
		if (param[0] > 16)
			return (false);
		else
			op1 = process->registre[param[0]];
	}
	else if (value == OCP_IND || value == OCP_DIR)
		op1 = param[0];
	else
		return (false);
	process->registre[param[2]] = op1 | op2;
	return (true);
}

bool		zjmp(t_vm *game, t_process *process, size_t *param, uint8_t ocp)
{
	int8_t value;

	if (process->cycle_to_do < 20)
		return (false);
	if (process->carry == false)
		return (false);
	value = (ocp >> 6) & 0b11;
	if (value != 2)
		return (false);
	process->offset = (process->offset + param[0]) % MEM_SIZE;
	return (true);
}

bool		sti(t_vm *game, t_process *process, size_t *param, uint8_t ocp)
{
	int8_t	value;
	int32_t op1;
	int32_t op2;
	char	*mem; //TODO

	if (process->cycle_to_do < 25)
		return (false);
	value = (ocp >> 2) & 0b11;
	if (value != OCP_REG && value != OCP_DIR)
		return (false);
	if (value == OCP_REG)
	{
		if (param[2] > 16)
			return (false);
		op2 = process->registre[param[2]];
	}
	else
		op2 = param[2];
	value = (ocp >> 4) & 0b11;
	if (ocp == 0)
		return (false);
	if (value == OCP_REG)
	{
		if (param[1] > 16)
			return (false);
		op2 = process->registre[param[1]];
	}
	else
		op2 = param[1];
	value = (ocp >> 6) & 0b11;
	if (value != OCP_REG || (value == OCP_REG && param[0] > 16))
		return (false);
	mem_write_int(mem, process->registre[param[0]], REG_SIZE, (op1 + op2) % MEM_SIZE);
	return (true);
}
