/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fcnt_09_12.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/04 13:27:13 by prastoin          #+#    #+#             */
/*   Updated: 2019/05/02 11:13:46 by fbecerri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"
#include "ft_string.h"

bool		zjmp(t_vm *vm, t_process *process, int32_t param[4], uint8_t ocp)
{
	ssize_t	offset;

	(void)ocp;
	(void)vm;
	if (vm->flags.verbose)
		ft_putf_fd(vm->v_fd, "P%5d | zjmp %d %s\n", vm->c_pc,
				param[0], process->carry == true ? "OK" : "FAILED");
	if (process->carry == false)
		return (invalid(vm, process, 0b11000000, 9));
	offset = (process->offset + param[0] % IDX_MOD);
	while (offset < 0)
		offset += MEM_SIZE;
	process->offset = offset % MEM_SIZE;
	if (vm->c_pc == 50)
		printf("has jmp to %ld\n", process->offset);
	return (true);
}

void		affldi(t_vm *vm, uint8_t op1[REG_SIZE], t_process *process,
		int32_t param[4])
{
	uint8_t		adr[REG_SIZE];
	int32_t		adress;

	bin_add(op1, process->tampon, adr);
	adress = (conv_bin_num(adr, REG_SIZE) % IDX_MOD);
	mem_read(vm->mem, process->registre[param[2] - 1],
			process->offset + adress, REG_SIZE);
	while (adress + process->offset < 0)
		adress += MEM_SIZE;
	if (vm->flags.verbose)
	{
		ft_putf_fd(vm->v_fd, "P%5d | ldi %D %D r%d\n", vm->c_pc,
				conv_bin_num(op1, REG_SIZE),
				conv_bin_num(process->tampon, REG_SIZE), param[2]);
		ft_putf_fd(vm->v_fd,
				"       | -> load from %D + %D = %D (with pc and mod %D)\n",
				conv_bin_num(op1, REG_SIZE),
				conv_bin_num(process->tampon, REG_SIZE),
				conv_bin_num(adr, REG_SIZE),
				(process->offset + adress) % MEM_SIZE);
	}
}

void		affsti(t_vm *vm, uint8_t op1[REG_SIZE], t_process *process,
		int32_t param[4])
{
	uint8_t		adr[REG_SIZE];
	int32_t		adress;

	bin_add(op1, process->tampon, adr);
	adress = (conv_bin_num(adr, REG_SIZE)) % IDX_MOD;
	mem_write(vm->mem, process->registre[param[0] - 1],
			process->offset + adress, REG_SIZE);
	if (vm->flags.verbose)
	{
		ft_putf_fd(vm->v_fd, "P%5d | sti r%d %D %D\n", vm->c_pc, param[0],
				conv_bin_num(op1, REG_SIZE), conv_bin_num(process->tampon,
					REG_SIZE));
		ft_putf_fd(vm->v_fd,
				"       | -> store to %D + %D = %D (with pc and mod %D)\n",
				conv_bin_num(op1, REG_SIZE), conv_bin_num(process->tampon,
					REG_SIZE), conv_bin_num(adr, REG_SIZE),
				process->offset + adress);
	}
}

bool		ldi(t_vm *vm, t_process *process, int32_t param[4], uint8_t ocp)
{
	uint8_t		op1[REG_SIZE];

	if (param[2] > 16 || param[0] <= 0)
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
	if (vm->vec->len == 49)
		printf("creating the 50 process the param[0] = %d - ancient offset = %ld\n", param[0], process->offset);
	while (param[0] + process->offset < 0)
	{
		printf("it was the case with the %ld process\n", vm->c_pc);
		param[0] += MEM_SIZE;
	}
	index = (process - vm->vec->processes);
	new_process = add_process(&vm->vec);
	process = vm->vec->processes + index;
	*new_process = (t_process) {
		.offset = (process->offset + param[0]) % MEM_SIZE,
		.is_alive = true,
	};
	if (vm->vec->len == 50)
		printf("true story is offset is %ld\n", new_process->offset);
	copy_process(new_process, process);
	if (vm->flags.verbose)
		ft_putf_fd(vm->v_fd, "P%5d | fork %d (%d)\n", vm->c_pc, save,
				(save % IDX_MOD) + process->offset);
	return (valid(vm, process, 0b11000000, 12));
}
