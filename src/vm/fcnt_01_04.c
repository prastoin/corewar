/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fcnt_01_04.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/04 13:22:08 by prastoin          #+#    #+#             */
/*   Updated: 2019/06/03 18:19:24 by prastoin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"
#include "ft_string.h"

bool		live(t_vm *vm, t_process *process, int32_t param[4], uint8_t ocp)
{
	int32_t		player;

	(void)ocp;
	player = param[0];
	player = -player;
	process->said_live = true;
	process->last_cycle_live = vm->cycle;
	hook_process_live(vm, process, player);
	if (player >= 1 && player <= MAX_PLAYERS)
	{
		if (vm->champ[player - 1].fd)
		{
			vm->champ[player - 1].last_cycle_live = vm->cycle;
		}
	}
	vm->nbr_live++;
	if (vm->flags.verbose)
		ft_putf_fd(vm->v_fd, "P %4d | live %d\n", vm->c_pc, param[0]);
	return (valid(vm, process, 0b10000000, 1));
}

bool		ld(t_vm *vm, t_process *process, int32_t param[4], uint8_t ocp)
{
	if (param[1] > 16 || param[1] <= 0)
		return (invalid(vm, process, ocp, 2));
	if (!ft_get_value_mod(param[0], ocp >> 6 & 0b11, process, vm))
		return (invalid(vm, process, ocp, 2));
	ft_memcpy(process->registre[param[1] - 1], process->tampon, REG_SIZE);
	if (vm->flags.verbose)
		ft_putf_fd(vm->v_fd, "P %4d | ld %D r%d\n", vm->c_pc,
			conv_bin_num(process->tampon, REG_SIZE), param[1]);
	if ((conv_bin_num(process->tampon, REG_SIZE)) == 0)
		return (carry_up(vm, process, ocp, 2));
	else
		return (carry_down(vm, process, ocp, 2));
}

bool		st(t_vm *vm, t_process *process, int32_t param[4], uint8_t ocp)
{
	if (param[0] > 16 || param[0] <= 0)
		return (invalid(vm, process, ocp, 3));
	if ((ocp >> 4 & 0b11) == OCP_REG)
	{
		if (param[1] > 16 || param[1] <= 0)
			return (invalid(vm, process, ocp, 3));
		else
			ft_memcpy(process->registre[param[1] - 1],
				process->registre[param[0] - 1], REG_SIZE);
	}
	else
	{
		mem_write(vm->mem, process->registre[param[0] - 1],
			(process->offset + (param[1] % IDX_MOD)), REG_SIZE);
		hook_process_memory_write(process, process->offset
			+ (param[1] % IDX_MOD), REG_SIZE);
	}
	if (vm->flags.verbose)
		ft_putf_fd(vm->v_fd, "P %4d | st r%d %d\n", vm->c_pc,
			param[0], param[1]);
	return (valid(vm, process, ocp, 3));
}

bool		add(t_vm *vm, t_process *process, int32_t param[4], uint8_t ocp)
{
	uint8_t op1[REG_SIZE];

	if (param[2] > 16 || param[2] <= 0)
		return (invalid(vm, process, ocp, 4));
	if (!(ft_get_value_mod(param[0], (ocp >> 6 & 0b11), process, vm)))
		return (invalid(vm, process, ocp, 4));
	ft_memcpy(op1, process->tampon, REG_SIZE);
	if (!(ft_get_value_mod(param[1], (ocp >> 4 & 0b11), process, vm)))
		return (invalid(vm, process, ocp, 4));
	bin_add(op1, process->tampon, process->registre[param[2] - 1]);
	if (vm->flags.verbose)
		ft_putf_fd(vm->v_fd, "P %4d | add r%d r%d r%d\n", vm->c_pc, param[0],
			param[1], param[2]);
	if ((conv_bin_num(process->registre[param[2] - 1], REG_SIZE)) == 0)
		return (carry_up(vm, process, ocp, 4));
	else
		return (carry_down(vm, process, ocp, 4));
}
