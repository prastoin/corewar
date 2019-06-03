/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fcnt_09_12.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/04 13:27:13 by prastoin          #+#    #+#             */
/*   Updated: 2019/05/23 16:22:34 by prastoin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"
#include "ft_string.h"

bool		zjmp(t_vm *vm, t_process *process, int32_t param[4], uint8_t ocp)
{
	ssize_t	offset;

	(void)ocp;
	(void)vm;
	offset = (process->offset + param[0] % IDX_MOD);
	while (offset < 0)
		offset += MEM_SIZE;
	offset %= MEM_SIZE;
	hook_process_jump(vm, process, param[0], process->carry ? offset : 0);
	if (process->carry == false)
		return (invalid(vm, process, 0b11000000, 9));
	process->offset = offset;
	return (true);
}

bool		ldi(t_vm *vm, t_process *process, int32_t param[4], uint8_t ocp)
{
	uint8_t		op1[REG_SIZE];

	if (param[2] > 16 || param[2] <= 0)
		return (invalid(vm, process, ocp, 10));
	if (!ft_get_value_mod(param[0], (ocp >> 6 & 0b11), process, vm))
		return (invalid(vm, process, ocp, 10));
	ft_memcpy(op1, process->tampon, REG_SIZE);
	if (!ft_get_value_mod(param[1], (ocp >> 4 & 0b11), process, vm))
		return (invalid(vm, process, ocp, 10));
	affldi(vm, op1, process, param);
	return (valid(vm, process, ocp, 10));
}

bool		sti(t_vm *vm, t_process *process, int32_t param[4], uint8_t ocp)
{
	uint8_t		op1[REG_SIZE];

	if (param[0] > 16 || param[0] <= 0)
		return (invalid(vm, process, ocp, 11));
	if (!ft_get_value_mod(param[1], (ocp >> 4 & 0b11), process, vm))
		return (invalid(vm, process, ocp, 11));
	ft_memcpy(op1, process->tampon, REG_SIZE);
	if (!ft_get_value(param[2], (ocp >> 2 & 0b11), process, vm))
		return (invalid(vm, process, ocp, 11));
	affsti(vm, op1, process, param);
	return (valid(vm, process, ocp, 11));
}

bool		ft_fork(t_vm *vm, t_process *process, int32_t param[4], uint8_t ocp)
{
	t_process	*new_process;
	size_t		index;
	int			save;

	(void)ocp;
	save = param[0];
	param[0] = param[0] % IDX_MOD;
	while (param[0] + process->offset < 0)
		param[0] += MEM_SIZE;
	index = (process - vm->vec->processes);
	if (!(new_process = add_process(&vm->vec)))
		return (false);
	process = vm->vec->processes + index;
	*new_process = (t_process) {
		.offset = (process->offset + param[0]) % MEM_SIZE,
		.is_alive = true,
		.cycle_to_do = 1
	};
	copy_process(new_process, process);
	hook_process_spawn(new_process, process, new_process->offset);
	if (vm->flags.verbose)
		ft_putf_fd(vm->v_fd, "P %4d | fork %d (%d)\n", vm->c_pc, save,
			(save % IDX_MOD) + process->offset);
	read_opcode(vm, new_process);
	return (valid(vm, process, 0b11000000, 12));
}
