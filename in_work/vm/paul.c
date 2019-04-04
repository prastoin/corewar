/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paul.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/03 11:18:42 by prastoin          #+#    #+#             */
/*   Updated: 2019/04/03 16:53:14 by prastoin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

#define REG_T 0b01
#define IND_T 0b11
#define DIR_T 0b10

bool	carry_up(t_process *process)
{
	process->carry = 1;
	process->cycle_to_do = 0;
	return (true);
}

bool	carry_down(t_process *process)
{
	process->carry = -1;
	process->cycle_to_do = 0;
	return (false);
}

bool		live(t_vm *game, t_process *process, size_t *param, uint8_t ocp)
{
	if (process->cycle_to_do < 10)
		return (false);
	process->cycle_to_do = 0;
	process->last_cycle_live = game->cycle;
	process->is_alive = 1;
	if (param[0] >= 1 && param[0] <= MAX_PLAYERS)
		game->live[param[0] - 1] = true;
	return (true);
}

//TODO LIVE OK

bool		ld(t_vm *game, t_process *process, size_t *param, uint8_t ocp)
{
	size_t value;

	if (process->cycle_to_do < 5)
		return (false);
	if (!ft_check_ocp(ocp, 2))
		return (carry_down(process));
	value = ft_get_value_mod(param[0], ocp >> 6 & 0b11, process, game);
	if (process->success == false || param[1] >= 16)
		return (carry_down(process));
	process->registre[param[1]] = value;
	return (carry_up(process));
}

//TODO LD OK

bool		lld(t_vm *game, t_process *process, size_t *param, uint8_t ocp)
{
	size_t value;

	if (process->cycle_to_do < 5)
		return (false);
	if (!ft_check_ocp(ocp, 2))
		return (carry_down(process));
	value = ft_get_value(param[0], ocp >> 6 & 0b11, process, game);
	if (process->success == false || param[1] >= 16)
		return (carry_down(process));
	process->registre[param[1]] = value;
	return (carry_up(process));
}

//TODO LLD OK

bool		and(t_vm *game, t_process *process, size_t *param, uint8_t ocp)
{
	size_t op1;
	size_t op2;

	if (process->cycle_to_do < 6)
		return (false);
	if (!ft_check_ocp(ocp, 6) || param[2] >= 16)
		return (carry_down(process));
	op1 = ft_get_value_mod(param[0], ocp << 6 & 0b11, process, game);
	if (process->success == false)
		return (carry_down(process));
	op2 = ft_get_value_mod(param[1], ocp << 4 & 0b11, process, game);
	if (process->success == false)
		return (carry_down(process));
	process->registre[param[2]] = op1 & op2;
	return (carry_up(process));
}

bool		or(t_vm *game, t_process *process, size_t *param, uint8_t ocp)
{
	size_t op1;
	size_t op2;

	if (process->cycle_to_do < 6)
		return (false);
	if (!ft_check_ocp(ocp, 7) || param[2] >= 16)
		return (carry_down(process));
	op1 = ft_get_value_mod(param[0], ocp << 6 & 0b11, process, game);
	if (process->success == false)
		return (carry_down(process));
	op2 = ft_get_value_mod(param[1], ocp << 4 & 0b11, process, game);
	if (process->success == false)
		return (carry_down(process));
	process->registre[param[2]] = op1 | op2;
	return (carry_up(process));
}

bool		xor(t_vm *game, t_process *process, size_t *param, uint8_t ocp)
{
	size_t op1;
	size_t op2;

	if (process->cycle_to_do < 6)
		return (false);
	if (!ft_check_ocp(ocp, 8) || param[2] >= 16)
		return (carry_down(process));
	op1 = ft_get_value_mod(param[0], ocp << 6 & 0b11, process, game);
	if (process->success == false)
		return (carry_down(process));
	op2 = ft_get_value_mod(param[1], ocp << 4 & 0b11, process, game);
	if (process->success == false)
		return (carry_down(process));
	process->registre[param[2]] = op1 ^ op2;
	return (carry_up(process));
}

//changer la taille des registre par uint32_t dans vm.h TODO
// ajouter mem dans s_vm TODO
bool		add(t_vm *game, t_process *process, size_t *param, uint8_t ocp)
{
	size_t op1;
	size_t op2;

	if (process->cycle_to_do < 10)
		return (false);
	if (!ft_check_ocp(ocp, 4) || param[2] >= 16)
		return (carry_down(process));
	op1 = ft_get_value_mod(param[0], ocp << 6 & 0b11, process, game);
	if (process->success == false)
		return (carry_down(process));
	op2 = ft_get_value_mod(param[1], ocp << 4 & 0b11, process, game);
	if (process->success == false)
		return (carry_down(process));
	process->registre[param[2]] = op1 + op2;
	return (carry_up(process));
}

bool		sub(t_vm *game, t_process *process, size_t *param, uint8_t ocp)
{
	size_t op1;
	size_t op2;

	if (process->cycle_to_do < 10)
		return (false);
	if (!ft_check_ocp(ocp, 5) || param[2] >= 16)
		return (carry_down(process));
	op1 = ft_get_value_mod(param[0], ocp << 6 & 0b11, process, game);
	if (process->success == false)
		return (carry_down(process));
	op2 = ft_get_value_mod(param[1], ocp << 4 & 0b11, process, game);
	if (process->success == false)
		return (carry_down(process));
	process->registre[param[2]] = op1 - op2;
	return (carry_up(process));
}

bool		ldi(t_vm *game, t_process *process, size_t *param, uint8_t ocp)
{
	uint32_t	op1;
	uint32_t	op2;
	uint8_t		str[8];

	if (process->cycle_to_do < 25)
		return (false);
	process->cycle_to_do = 0;
	if (!ft_check_ocp(ocp, 10) || param[2] >= 16)
		return (false);
	op1 = ft_get_value_mod(param[0], ocp << 6 & 0b11, process, game);
	if (process->success == false)
		return (carry_down(process));
	op2 = ft_get_value_mod(param[1], ocp << 4 & 0b11, process, game);
	if (process->success == false)
		return (carry_down(process));
	process->registre[param[2]] = (int16_t)mem_read(game->mem, str, (process->offset + (op1 + op2) % IDX_MOD) % MEM_SIZE, 2);
	return (true);
}

bool		lldi(t_vm *game, t_process *process, size_t *param, uint8_t ocp)
{
	uint32_t	op1;
	uint32_t	op2;
	uint8_t		str[8];

	if (process->cycle_to_do < 25)
		return (false);
	process->cycle_to_do = 0;
	if (!ft_check_ocp(ocp, 14) || param[2] >= 16)
		return (carry_down(process));
	op1 = ft_get_value(param[0], ocp << 6 & 0b11, process, game);
	if (process->success == false)
		return (carry_down(process));
	op2 = ft_get_value(param[1], ocp << 4 & 0b11, process, game);
	if (process->success == false)
		return (carry_down(process));
	process->registre[param[2]] = (int16_t)mem_read(game->mem, str, (process->offset + op1 + op2) % MEM_SIZE, 2);
	return (carry_up(process));
}
