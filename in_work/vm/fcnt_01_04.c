/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fcnt_01_04.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/04 13:22:08 by prastoin          #+#    #+#             */
/*   Updated: 2019/04/05 17:22:09 by prastoin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

bool		live(t_vm *game, t_process *process, size_t *param, uint8_t ocp)
{

	process->said_live = true;
	if (param[0] >= 1 && param[0] <= MAX_PLAYERS)
		game->live[param[0] - 1] = true;
	game->nbr_live++;
	game->champ[param[0]].last_cycle_live = game->cycle;
	return (valid(process, 0b10, 1));
}

bool		ld(t_vm *game, t_process *process, size_t *param, uint8_t ocp)
{
	if (param[1] >= 16)
		return (carry_down(process));
	if (!ft_get_value_mod(param[0], ocp >> 6 & 0b11, process, game))
		return (carry_down(process));
	ft_memcpy(process->registre[param[1]], process->tampon, REG_SIZE);
	return (carry_up(process, ocp, 2));
}

bool		st(t_vm *game, t_process *process, size_t *param, uint8_t ocp)
{
	if (param[0] >= 16)
		return (carry_down(process));
	if ((ocp >> 4 & 0b11) == OCP_REG)
	{
		if (param[1] >= 16)
			return (invalid(process));
		else
			ft_memcpy(process->registre[param[1]],
					process->registre[param[0]], REG_SIZE);
	}
	else
		mem_write(game->mem, process->registre[param[0]],
				(process->offset + (param[1] % IDX_MOD)) % MEM_SIZE, REG_SIZE);
	return (valid(process, ocp, 3));
}

bool		add(t_vm *game, t_process *process, size_t *param, uint8_t ocp)
{
	uint8_t op1[REG_SIZE];

	if (param[2] >= 16)
		return (carry_down(process));
	if (!(ft_get_value_mod(param[0], (ocp >> 6 & 0b11), process, game)))
		return (carry_down(process));
	ft_memcpy(op1, process->tampon, REG_SIZE);
	if (!(ft_get_value_mod(param[1], (ocp >> 4 & 0b11), process, game)))
		return (carry_down(process));
	if (!(bin_add(op1, process->tampon, process->registre[param[2]])))
		return (carry_down(process));
	return (carry_up(process, ocp, 4));
}
