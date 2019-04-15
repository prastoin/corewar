/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fcnt_09_12.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/04 13:27:13 by prastoin          #+#    #+#             */
/*   Updated: 2019/04/09 15:03:50 by prastoin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

bool		zjmp(t_vm *game, t_process *process, size_t *param, uint8_t ocp)
{
	(void)ocp;
	(void)game;
	if (process->carry == false)
		return (invalid(process)); //decaler seulement de un ?????? car pas un pb de parse #DAVID NEEDS TO WORKS
	process->offset = (process->offset + param[0] - 1) % MEM_SIZE;
	printf ("the new offset is %zu", process->offset);
	return (true); //TODO IDX MOD ICI ?
}

bool		ldi(t_vm *game, t_process *process, size_t *param, uint8_t ocp)
{
	uint8_t		op1[REG_SIZE];
	uint8_t		adr[REG_SIZE];
	uint64_t	adress;

	if (param[2] >= 16)
		return (invalid(process));
	if (!ft_get_value_mod(param[0], (ocp >> 6 & 0b11), process, game))
		return (invalid(process));
	ft_memcpy(op1, process->tampon, REG_SIZE);
	if (!ft_get_value_mod(param[1], (ocp >> 4 & 0b11), process, game))
		return (invalid(process));
	bin_add(op1, process->tampon, adr);
	adress = (conv_bin_num(adr, REG_SIZE) % IDX_MOD);
	mem_read(game->mem, process->registre[param[2]], (process->offset + adress) % MEM_SIZE, REG_SIZE);
	return (valid(process, ocp, 10));
}

bool		sti(t_vm *game, t_process *process, size_t *param, uint8_t ocp)
{
	uint8_t		op1[REG_SIZE];
	uint8_t		adr[REG_SIZE];
	uint64_t	adress;

	if (param[0] >= 16)
	{
		printf("\033[31m bad register %zu \033[0m ", param[0]);
		return (carry_down(process));
	}
	if (!ft_get_value_mod(param[1], (ocp >> 4 & 0b11), process, game))
		return (carry_down(process));
	ft_memcpy(op1, process->tampon, REG_SIZE);
	if (!ft_get_value_mod(param[2], (ocp >> 2 & 0b11), process, game))
		return (carry_down(process));
	bin_add(op1, process->tampon, adr);
	adress = (conv_bin_num(adr, REG_SIZE));
	printf("\n\t\t \033[32m └─copy to %zu registre = %.2x - %.2x - %.2x - %.2x\033[0m", adress, process->registre[param[0]][0], process->registre[param[0]][1], process->registre[param[0]][2], process->registre[param[0]][3]);
	mem_write(game->mem, process->registre[param[0]], (process->offset + adress - 2) % MEM_SIZE, REG_SIZE);
	ft_dump_mem(*game, false);
	return (carry_up(process, ocp, 11));
}

bool		ft_fork(t_vm *game, t_process *process, size_t *param, uint8_t ocp)
{
	t_process	new_process;
	size_t		old;

	(void)ocp;
	old = process->curr;
	printf("param[0] = %d\n", param[0]);
	new_process = add_process(&(game->vec), (process->offset + (param[0] % IDX_MOD)) % MEM_SIZE);
	process = game->vec->processes + old;
	copy_process(&new_process, process);
	printf("old = %d capacity = %d\n", old, game->vec->capacity);
	return (valid(process, 0b11000000, 12));
}
