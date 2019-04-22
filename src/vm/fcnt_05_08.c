/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fcnt_05_08.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/04 13:24:34 by prastoin          #+#    #+#             */
/*   Updated: 2019/04/22 15:30:09 by prastoin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

bool		sub(t_vm *game, t_process *process, int32_t param[4], uint8_t ocp)
{
	uint8_t op1[REG_SIZE];

	if (param[2] >= 16 || param[2] < 0)
		return (carry_down(process, ocp, 5));
	if (!(ft_get_value_mod(param[0], (ocp >> 6 & 0b11), process, game)))
		return (carry_down(process, ocp, 5));
	ft_memcpy(op1, process->tampon, REG_SIZE);
	if (!(ft_get_value_mod(param[1], (ocp >> 4 & 0b11), process, game)))
		return (carry_down(process, ocp, 5));
	if (!(bin_sub(op1, process->tampon, process->registre[param[2]])))
		return (carry_down(process, ocp, 5));
	return (carry_up(process, ocp, 5));
}

bool		ft_and(t_vm *game, t_process *process, int32_t param[4], uint8_t ocp)
{
	uint8_t op1[REG_SIZE];

	if (param[2] >= 16 || param[2] < 0)
		return (carry_down(process, ocp, 6));
	if (!(ft_get_value_mod(param[0], (ocp >> 6 & 0b11), process, game)))
		return (carry_down(process, ocp, 6));
	ft_memcpy(op1, process->tampon, REG_SIZE);
	if (!(ft_get_value_mod(param[1], (ocp >> 4 & 0b11), process, game)))
		return (carry_down(process, ocp, 6));
	bin_and(op1, process->tampon, process->registre[param[2]]);
	return (carry_up(process, ocp, 6));
}

bool		ft_or(t_vm *game, t_process *process, int32_t param[4], uint8_t ocp)
{
	uint8_t op1[REG_SIZE];

	if (param[2] >= 16 || param[2] < 0)
		return (carry_down(process, ocp, 7));
	if (!(ft_get_value_mod(param[0], (ocp >> 6 & 0b11), process, game)))
		return (carry_down(process, ocp, 7));
	ft_memcpy(op1, process->tampon, REG_SIZE);
	if (!(ft_get_value_mod(param[1], (ocp >> 4 & 0b11), process, game)))
		return (carry_down(process, ocp, 7));
	bin_or(op1, process->tampon, process->registre[param[2]]);
	return (carry_up(process, ocp, 7));
}

bool		ft_xor(t_vm *game, t_process *process, int32_t param[4], uint8_t ocp)
{
	uint8_t op1[REG_SIZE];

	if (param[2] >= 16 || param[2] < 0)
		return (carry_down(process, ocp, 8));
	if (!(ft_get_value_mod(param[0], (ocp >> 6 & 0b11), process, game)))
		return (carry_down(process, ocp, 8));
	ft_memcpy(op1, process->tampon, REG_SIZE);
	if (!(ft_get_value_mod(param[1], (ocp >> 4 & 0b11), process, game)))
		return (carry_down(process, ocp, 8));
	bin_xor(op1, process->tampon, process->registre[param[2]]);
	return (carry_up(process, ocp, 8));
}
