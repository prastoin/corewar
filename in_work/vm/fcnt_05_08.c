/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fcnt_05_08.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/04 13:24:34 by prastoin          #+#    #+#             */
/*   Updated: 2019/04/05 16:42:25 by prastoin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

bool		sub(t_vm *game, t_process *process, size_t *param, uint8_t ocp)
{
	uint8_t op1[REG_SIZE];

	if (param[2] >= 16)
		return (carry_down(process));
	if (!(ft_get_value_mod(param[0], (ocp >> 6 & 0b11), process, game)))
		return (carry_down(process));
	ft_memcpy(op1, process->tampon, REG_SIZE);
	if (!(ft_get_value_mod(param[1], (ocp >> 4 & 0b11), process, game)))
		return (carry_down(process));
	if (!(bin_sub(op1, process->tampon, process->registre[param[2]])))
		return (carry_down(process));
	return (carry_up(process, ocp, 5));
}

bool		and(t_vm *game, t_process *process, size_t *param, uint8_t ocp)
{
	uint8_t op1[REG_SIZE];

	if (param[2] >= 16)
		return (carry_down(process));
	if (!(ft_get_value_mod(param[0], (ocp >> 6 & 0b11), process, game)))
		return (carry_down(process));
	ft_memcpy(op1, process->tampon, REG_SIZE);
	if (!(ft_get_value_mod(param[1], (ocp >> 4 & 0b11), process, game)))
		return (carry_down(process));
	if (!(bin_and(op1, process->tampon, process->registre[param[2]])))
		return (carry_down(process));
	return (carry_up(process, ocp, 6));
}

bool		or(t_vm *game, t_process *process, size_t *param, uint8_t ocp)
{
	uint8_t op1[REG_SIZE];

	if (param[2] >= 16)
		return (carry_down(process));
	if (!(ft_get_value_mod(param[0], (ocp >> 6 & 0b11), process, game)))
		return (carry_down(process));
	ft_memcpy(op1, process->tampon, REG_SIZE);
	if (!(ft_get_value_mod(param[1], (ocp >> 4 & 0b11), process, game)))
		return (carry_down(process));
	if (!(bin_or(op1, process->tampon, process->registre[param[2]])))
		return (carry_down(process));
	return (carry_up(process, ocp, 7));
}

bool		xor(t_vm *game, t_process *process, size_t *param, uint8_t ocp)
{
	uint8_t op1[REG_SIZE];

	if (param[2] >= 16)
		return (carry_down(process));
	if (!(ft_get_value_mod(param[0], (ocp >> 6 & 0b11), process, game)))
		return (carry_down(process));
	ft_memcpy(op1, process->tampon, REG_SIZE);
	if (!(ft_get_value_mod(param[1], (ocp >> 4 & 0b11), process, game)))
		return (carry_down(process));
	if (!(bin_xor(op1, process->tampon, process->registre[param[2]])))
		return (carry_down(process));
	return (carry_up(process, ocp, 8));
}
