/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fcnt_05_08.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/04 13:24:34 by prastoin          #+#    #+#             */
/*   Updated: 2019/05/07 16:05:30 by prastoin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"
#include "ft_string.h"

bool		sub(t_vm *vm, t_process *process, int32_t param[4], uint8_t ocp)
{
	uint8_t op1[REG_SIZE];

	if (param[2] > 16 || param[2] <= 0)
		return (invalid(vm, process, ocp, 5));
	if (!(ft_get_value_mod(param[0], (ocp >> 6 & 0b11), process, vm)))
		return (invalid(vm, process, ocp, 5));
	ft_memcpy(op1, process->tampon, REG_SIZE);
	if (!(ft_get_value_mod(param[1], (ocp >> 4 & 0b11), process, vm)))
		return (invalid(vm, process, ocp, 5));
	bin_sub(op1, process->tampon, process->registre[param[2] - 1]);
	if (vm->flags.verbose)
		ft_putf_fd(vm->v_fd, "P %4d | sub r%d r%d r%d\n", vm->c_pc, param[0],
				param[1], param[2]);
	if ((conv_bin_num(process->registre[param[2] - 1], REG_SIZE)) == 0)
		return (carry_up(vm, process, ocp, 5));
	else
		return (carry_down(vm, process, ocp, 5));
}

bool		ft_and(t_vm *vm, t_process *process, int32_t param[4], uint8_t ocp)
{
	uint8_t op1[REG_SIZE];

	if (param[2] > 16 || param[2] <= 0)
		return (invalid(vm, process, ocp, 6));
	if (!(ft_get_value_mod(param[0], (ocp >> 6 & 0b11), process, vm)))
		return (invalid(vm, process, ocp, 6));
	ft_memcpy(op1, process->tampon, REG_SIZE);
	if (!(ft_get_value_mod(param[1], (ocp >> 4 & 0b11), process, vm)))
		return (invalid(vm, process, ocp, 6));
	bin_and(op1, process->tampon, process->registre[param[2] - 1]);
	if (vm->flags.verbose)
		ft_putf_fd(vm->v_fd, "P %4d | and %d %d r%d\n", vm->c_pc,
				conv_bin_num(op1, REG_SIZE), conv_bin_num(process->tampon,
					REG_SIZE), param[2]);
	if ((conv_bin_num(process->registre[param[2] - 1], REG_SIZE)) == 0)
		return (carry_up(vm, process, ocp, 6));
	else
		return (carry_down(vm, process, ocp, 6));
}

bool		ft_or(t_vm *vm, t_process *process, int32_t param[4], uint8_t ocp)
{
	uint8_t op1[REG_SIZE];

	if (param[2] > 16 || param[2] <= 0)
		return (invalid(vm, process, ocp, 7));
	if (!(ft_get_value_mod(param[0], (ocp >> 6 & 0b11), process, vm)))
		return (invalid(vm, process, ocp, 7));
	ft_memcpy(op1, process->tampon, REG_SIZE);
	if (!(ft_get_value_mod(param[1], (ocp >> 4 & 0b11), process, vm)))
		return (invalid(vm, process, ocp, 7));
	bin_or(op1, process->tampon, process->registre[param[2] - 1]);
	if (vm->flags.verbose)
		ft_putf_fd(vm->v_fd, "P %4d | or %d %d r%d\n", vm->c_pc,
				conv_bin_num(op1, REG_SIZE), conv_bin_num(process->tampon,
					REG_SIZE), param[2]);
	if ((conv_bin_num(process->registre[param[2] - 1], REG_SIZE)) == 0)
		return (carry_up(vm, process, ocp, 7));
	else
		return (carry_up(vm, process, ocp, 7));
}

bool		ft_xor(t_vm *vm, t_process *process, int32_t param[4], uint8_t ocp)
{
	uint8_t op1[REG_SIZE];

	if (param[2] > 16 || param[2] <= 0)
		return (invalid(vm, process, ocp, 8));
	if (!(ft_get_value_mod(param[0], (ocp >> 6 & 0b11), process, vm)))
		return (invalid(vm, process, ocp, 8));
	ft_memcpy(op1, process->tampon, REG_SIZE);
	if (!(ft_get_value_mod(param[1], (ocp >> 4 & 0b11), process, vm)))
		return (invalid(vm, process, ocp, 8));
	bin_xor(op1, process->tampon, process->registre[param[2] - 1]);
	if (vm->flags.verbose)
		ft_putf_fd(vm->v_fd, "P %4d | xor %d %d r%d\n", vm->c_pc,
				conv_bin_num(op1, REG_SIZE), conv_bin_num(process->tampon,
					REG_SIZE), param[2]);
	if ((conv_bin_num(process->registre[param[2] - 1], REG_SIZE)) == 0)
		return (carry_up(vm, process, ocp, 8));
	else
		return (carry_down(vm, process, ocp, 8));
}
