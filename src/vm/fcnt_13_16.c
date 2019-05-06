/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fcnt_13_16.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/04 13:31:01 by prastoin          #+#    #+#             */
/*   Updated: 2019/05/06 03:00:08 by fbecerri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"
#include "ft_string.h"
#include "stdio.h"

bool		lld(t_vm *vm, t_process *process, int32_t param[4], uint8_t ocp)
{
	if (param[1] > 16 || param[1] <= 0)
		return (invalid(vm, process, ocp, 13));
	if (!ft_get_value(param[0], ocp >> 6 & 0b11, process, vm))
		return (invalid(vm, process, ocp, 13));
	ft_memcpy(process->registre[param[1] - 1], process->tampon, REG_SIZE);
	if (vm->flags.verbose)
		ft_putf_fd(vm->v_fd, "P%5d | lld %D r%d\n", vm->c_pc,
				conv_bin_num(process->tampon, REG_SIZE), param[1]);
	if ((conv_bin_num(process->tampon, REG_SIZE)) == 0)
		return (carry_up(vm, process, ocp, 13));
	else
		return (carry_down(vm, process, ocp, 13));
}

bool		lldi(t_vm *vm, t_process *process, int32_t param[4], uint8_t ocp)
{
	uint8_t		op1[REG_SIZE];
	uint8_t		adr[REG_SIZE];
	int64_t		adress;

	if (param[2] > 16 || param[2] <= 0)
		return (invalid(vm, process, ocp, 14));
	if (!ft_get_value_mod(param[0], (ocp >> 6 & 0b11), process, vm))
		return (invalid(vm, process, ocp, 14));
	ft_memcpy(op1, process->tampon, REG_SIZE);
	if (!ft_get_value_mod(param[1], (ocp >> 4 & 0b11), process, vm))
		return (invalid(vm, process, ocp, 14));
	bin_add(op1, process->tampon, adr);
	adress = (conv_bin_num(adr, REG_SIZE));
	while ((process->offset + adress) < 0)
		adress += MEM_SIZE;
	mem_read(vm->mem, process->registre[param[2] - 1],
			(process->offset + adress) % MEM_SIZE, REG_SIZE);
	if (vm->flags.verbose)
		print_a(vm, op1, process->tampon, param[2]);
	if (vm->flags.verbose)
		print_b(vm, process, adr, adress);
	if ((conv_bin_num(process->registre[param[2] - 1], REG_SIZE)) == 0)
		return (carry_up(vm, process, ocp, 14));
	else
		return (carry_down(vm, process, ocp, 14));
}

bool		lfork(t_vm *vm, t_process *process, int32_t param[4], uint8_t ocp)
{
	t_process	*new_process;
	size_t		index;
	int			save;

	(void)ocp;
	save = param[0];
	while (param[0] + process->offset < 0)
		param[0] += MEM_SIZE;
	index = (process - vm->vec->processes);
	new_process = add_process(&vm->vec);
	process = vm->vec->processes + index;
	*new_process = (t_process) {
		.offset = (process->offset + param[0]) % MEM_SIZE,
		.is_alive = true,
	};
	copy_process(new_process, process);
	if (vm->flags.verbose)
		ft_putf_fd(vm->v_fd, "P%5d | lfork %d (%d)\n", vm->c_pc, save,
				save + process->offset);
	return (valid(vm, process, 0b11000000, 15));
}

bool		aff(t_vm *vm, t_process *process, int32_t param[4], uint8_t ocp)
{
	size_t		i;
	uint8_t		c;

	(void)vm;
	if (param[0] > 16 || param[0] <= 0)
		return (invalid(vm, process, ocp, 16));
	i = 0;
	while (i < REG_SIZE)
	{
		c = (process->registre[param[0] - 1][i] % 256);
		write(1, &c, 1);
		i++;
	}
	return (valid(vm, process, ocp, 16));
}
