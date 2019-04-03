/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   op.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/14 14:32:56 by prastoin          #+#    #+#             */
/*   Updated: 2019/04/03 11:17:03 by prastoin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

bool		live(t_vm *game, t_process *process, size_t *param, uint8_t ocp);
bool		ld(t_vm *game, t_process *process, size_t *param, uint8_t ocp);
bool		st(t_vm *game, t_process *process, size_t *param, uint8_t ocp);
bool		add(t_vm *game, t_process *process, size_t *param, uint8_t ocp);
bool		sub(t_vm *game, t_process *process, size_t *param, uint8_t ocp);
bool		and(t_vm *game, t_process *process, size_t *param, uint8_t ocp);
bool		or(t_vm *game, t_process *process, size_t *param, uint8_t ocp);
bool		xor(t_vm *game, t_process *process, size_t *param, uint8_t ocp);
bool		zjmp(t_vm *game, t_process *process, size_t *param, uint8_t ocp);
bool		ldi(t_vm *game, t_process *process, size_t *param, uint8_t ocp);
bool		sti(t_vm *game, t_process *process, size_t *param, uint8_t ocp);
bool		ft_fork(t_vm *game, t_process *process, size_t *param, uint8_t ocp);
bool		lld(t_vm *game, t_process *process, size_t *param, uint8_t ocp);
bool		lldi(t_vm *game, t_process *process, size_t *param, uint8_t ocp);
bool		lfork(t_vm *game, t_process *process, size_t *param, uint8_t ocp);
bool		aff(t_vm *game, t_process *process, size_t *param, uint8_t ocp);

t_core_fcnt g_fcnt[17] = {
	[0x01] = live,
	[0x02] = ld,
	[0x03] = st,
	[0x04] = add,
	[0x05] = sub,
	[0x06] = and,
	[0x07] = or,
	[0x08] = xor,
	[0x09] = zjmp,
	[0x0a] = ldi,
	[0x0b] = sti,
	[0x0c] = ft_fork,
	[0x0d] = lld,
	[0x0e] = lldi,
	[0x0f] = lfork,
	[0x10] = aff
};

bool		live(t_vm *game, t_process *process, size_t *param, uint8_t ocp)
{
	if (process->cycle_to_do < 10)
		return (false);
	else if (param[0] == process->nb_player)
	{
		process->cycle_to_do = 0;
		process->last_cycle_live = game->cycle;
		process->is_alive = 1;
	}
	return (true);
}

bool		ld(t_vm *game, t_process *process, size_t *param, uint8_t ocp)
{
	if (param[1] < 16)
		return (false);
	process->registre[param[0]] = param[1];
	return (true);
}
