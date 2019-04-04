/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fcnt_09_12.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/04 13:27:13 by prastoin          #+#    #+#             */
/*   Updated: 2019/04/04 17:02:03 by prastoin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

bool		zjmp(t_vm *game, t_process *process, size_t *param, uint8_t ocp)
{
	if (process->carry == false)
		return (invalid(process)); //decaler seulement de un ?????? car pas un pb de parse #DAVID NEEDS TO WORKS
	process->offset = (process->offset + param[0]) % MEM_SIZE;
	return (true); //TODO IDX MOD ICI ?
}

bool		ldi(t_vm *game, t_process *process, size_t *param, uint8_t ocp)
{
	uint8_t		op1[REG_SIZE];
	uint8_t		adr[REG_SIZE];
	uint64_t	adress;

	if (!ft_check_ocp(ocp, 10) || param[2] >= 16)
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

	if (!ft_check_ocp(ocp, 11) || param[0] >= 16)
		return (carry_down(process));
	if (!ft_get_value_mod(param[1], (ocp >> 4 & 0b11), process, game))
		return (carry_down(process));
	ft_memcpy(op1, process->tampon, REG_SIZE);
	if (!ft_get_value_mod(param[2], (ocp >> 2 & 0b11), process, game))
		return (carry_down(process));
	bin_add(op1, process->tampon, adr);
	adress = (conv_bin_num(adr, REG_SIZE) % IDX_MOD);
	mem_write(game->mem, process->registre[param[0]], (process->offset + adress) % MEM_SIZE, REG_SIZE);
	return (carry_up(process, ocp, 11));
}

bool		ft_fork(t_vm *game, t_process *process, size_t *param, uint8_t ocp)
{
	t_process new_process;

	new_process = add_process(&(game->vec), (process->offset + (param[1] % IDX_MOD)) % MEM_SIZE);
	copy_process(&new_process, process);
	return (valid(process, 0b11, 12));
}
