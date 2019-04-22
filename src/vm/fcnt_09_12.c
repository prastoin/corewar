/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fcnt_09_12.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/04 13:27:13 by prastoin          #+#    #+#             */
/*   Updated: 2019/04/22 18:42:45 by prastoin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

bool		zjmp(t_vm *game, t_process *process, int32_t param[4], uint8_t ocp)
{
	ssize_t	offset;
	(void)ocp;
	(void)game;
	if (process->carry == false)
		return (invalid(process, ocp, 9));
	offset = (process->offset + /*(int32_t)*/param[0] % IDX_MOD);
	while (offset < 0)
		offset += MEM_SIZE;
	process->offset = offset % MEM_SIZE;
	printf ("param %d\n", param[0]);
	printf ("the new offset is %zu", process->offset);
	return (true);
}

void		ft_dump_process(t_process *process)
{
	printf("Process:\n");
	for(size_t i = 0; i < REG_NUMBER; i++)
		printf("\tReg %zu: %d, %x-%x-%x-%x\n", i, conv_bin_num(process->registre[i], REG_SIZE), process->registre[i][0], process->registre[i][1], process->registre[i][2], process->registre[i][3]);
}

bool		ldi(t_vm *game, t_process *process, int32_t param[4], uint8_t ocp)
{
	uint8_t		op1[REG_SIZE];
	uint8_t		adr[REG_SIZE];
	uint64_t	adress;

	if (param[2] >= 16 || param[0] < 0)
		return (invalid(process, ocp, 10));
	if (!ft_get_value_mod(param[0], (ocp >> 6 & 0b11), process, game))
		return (invalid(process, ocp, 10));
	ft_memcpy(op1, process->tampon, REG_SIZE);
	if (!ft_get_value_mod(param[1], (ocp >> 4 & 0b11), process, game))
		return (invalid(process, ocp, 10));
	bin_add(op1, process->tampon, adr);
	printf("Lol lol %d + %d: %d + %d = %d", param[1], param[2], conv_bin_num(op1, REG_SIZE), conv_bin_num(process->tampon, REG_SIZE), conv_bin_num(adr, REG_SIZE));
	adress = (conv_bin_num(adr, REG_SIZE) % IDX_MOD);
	printf("Address: %d\n", adress);
	mem_read(game->mem, process->registre[param[2]], process->offset + adress, REG_SIZE);
	ft_dump_process(process);
	return (valid(process, ocp, 10));
}

bool		sti(t_vm *game, t_process *process, int32_t param[4], uint8_t ocp)
{
	uint8_t		op1[REG_SIZE];
	uint8_t		adr[REG_SIZE];
	intmax_t	adress;

	if (param[0] >= 16 || param[0] < 0)
	{
		printf("\033[31m bad register %zu \033[0m ", param[0]);
		return (invalid(process, ocp, 11));
	}
	if (!ft_get_value_mod(param[1], (ocp >> 4 & 0b11), process, game))
		return (invalid(process, ocp, 11));
	ft_memcpy(op1, process->tampon, REG_SIZE);
	if (!ft_get_value(param[2], (ocp >> 2 & 0b11), process, game))
		return (invalid(process, ocp, 11));
	bin_add(op1, process->tampon, adr);
	adress = (conv_bin_num(adr, REG_SIZE))% IDX_MOD;
	printf("Lol %d + %d: %d + %d = %d", param[1], param[2], conv_bin_num(op1, REG_SIZE), conv_bin_num(process->tampon, REG_SIZE), conv_bin_num(adr, REG_SIZE));
	printf("\n\t\t \033[32m └─copy to %d registre = %.2x - %.2x - %.2x - %.2x\033[0m", adress, process->registre[param[0]][0], process->registre[param[0]][1], process->registre[param[0]][2], process->registre[param[0]][3]);
	mem_write(game->mem, process->registre[param[0]], process->offset + adress, REG_SIZE);
	printf("With pc %d\n", process->offset + adress);
	ft_dump_mem(*game, false);
	ft_dump_process(process);
	if (conv_bin_num(process->registre[param[0]], REG_SIZE) == 0)
		return (carry_up(process, ocp, 11));
	else
		return (carry_down(process, ocp, 11));
}

bool		ft_fork(t_vm *game, t_process *process, int32_t param[4], uint8_t ocp)
{
	t_process	*new_process;
	size_t		index;

	printf("param[0] = %d\n", param[0]);
	while (param[0] + process->offset < 0)
		param[0] += MEM_SIZE;
	index = (process - game->vec->processes);
	new_process = add_process(&game->vec);
	process = game->vec->processes + index;
	*new_process = (t_process) {
		.offset = (process->offset + (param[0] % IDX_MOD)) % MEM_SIZE, // TODO: Negative offset
		.is_alive = true
	};
	copy_process(new_process, process);
	printf("Fork process[%d] creating %d\n", index, new_process - game->vec->processes);
	return (valid(process, 0b11000000, 12));
}
