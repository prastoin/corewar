/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aff_fcnt_vm.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbecerri <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/06 01:34:18 by fbecerri          #+#    #+#             */
/*   Updated: 2019/06/05 10:00:15 by dde-jesu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

void	print_a(t_vm *vm, uint8_t op1[REG_SIZE], uint8_t tampon[REG_SIZE],
		int32_t param)
{
	io_putf(&vm->v, "P %4d | lldi %D %D r%D\n       | -> load from %d",
		vm->c_pc, conv_bin_num(op1, REG_SIZE), conv_bin_num(tampon,
			REG_SIZE), param, conv_bin_num(op1, REG_SIZE));
}

void	print_b(t_vm *vm, t_process *process, uint8_t adr[REG_SIZE],
		int64_t adress)
{
	io_putf(&vm->v, " + %D = %D (with pc %D)\n",
		conv_bin_num(process->tampon, REG_SIZE), conv_bin_num(adr,
			REG_SIZE), (process->offset + adress) % MEM_SIZE);
}

void	affldi(t_vm *vm, uint8_t op1[REG_SIZE], t_process *process,
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
		io_putf(&vm->v, "P %4d | ldi %D %D r%d\n", vm->c_pc,
			conv_bin_num(op1, REG_SIZE),
			conv_bin_num(process->tampon, REG_SIZE), param[2]);
		io_putf(&vm->v,
			"       | -> load from %D + %D = %D (with pc and mod %D)\n",
			conv_bin_num(op1, REG_SIZE),
			conv_bin_num(process->tampon, REG_SIZE),
			conv_bin_num(adr, REG_SIZE),
			(process->offset + adress) % MEM_SIZE);
	}
}

void	affsti(t_vm *vm, uint8_t op1[REG_SIZE], t_process *process,
		int32_t param[4])
{
	uint8_t		adr[REG_SIZE];
	int32_t		adress;

	bin_add(op1, process->tampon, adr);
	adress = (conv_bin_num(adr, REG_SIZE)) % IDX_MOD;
	mem_write(vm->mem, process->registre[param[0] - 1],
		process->offset + adress, REG_SIZE);
	hook_process_memory_write(process, process->offset + adress, REG_SIZE);
	if (vm->flags.verbose)
	{
		io_putf(&vm->v, "P %4d | sti r%d %D %D\n", vm->c_pc, param[0],
			conv_bin_num(op1, REG_SIZE), conv_bin_num(process->tampon,
				REG_SIZE));
		io_putf(&vm->v,
			"       | -> store to %D + %D = %D (with pc and mod %D)\n",
			conv_bin_num(op1, REG_SIZE), conv_bin_num(process->tampon,
				REG_SIZE), conv_bin_num(adr, REG_SIZE),
			process->offset + adress);
	}
}
