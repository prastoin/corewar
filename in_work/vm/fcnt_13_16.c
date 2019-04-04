/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fcnt_13_16.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/04 13:31:01 by prastoin          #+#    #+#             */
/*   Updated: 2019/04/04 17:01:32 by prastoin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

bool		lld(t_vm *game, t_process *process, size_t *param, uint8_t ocp)
{
	if (!ft_check_ocp(ocp, 13))
		return (carry_down(process));
	if (param[1] >= 16)
		return (carry_down(process));
	if (!ft_get_value(param[0], ocp >> 6 & 0b11, process, game))
		return (carry_down(process));
	ft_memcpy(process->registre[param[1]], process->tampon, REG_SIZE);
	return (carry_up(process, ocp, 13));
}

bool		lldi(t_vm *game, t_process *process, size_t *param, uint8_t ocp)
{
	uint8_t		op1[REG_SIZE];
	uint8_t		adr[REG_SIZE];
	uint64_t	adress;

	if (!ft_check_ocp(ocp, 14) || param[2] >= 16)
		return (carry_down(process));
	if (!ft_get_value(param[0], (ocp >> 6 & 0b11), process, game))
		return (carry_down(process));
	ft_memcpy(op1, process->tampon, REG_SIZE);
	if (!ft_get_value(param[1], (ocp >> 4 & 0b11), process, game))
		return (carry_down(process));
	bin_add(op1, process->tampon, adr);
	adress = (conv_bin_num(adr, REG_SIZE));
	mem_read(game->mem, process->registre[param[2]], (process->offset + adress) % MEM_SIZE, REG_SIZE);
	return (carry_up(process, ocp, 14));
}

bool		lfork(t_vm *game, t_process *process, size_t *param, uint8_t ocp)
{
	t_process new_process;

	new_process = add_process(&(game->vec), (process->offset + param[1] % MEM_SIZE));
	copy_process(&new_process, process);
	return (valid(process, 0b11, 15));
}

bool		aff(t_vm *game, t_process *process, size_t *param, uint8_t ocp)
{
	size_t		i;
	uint8_t		c;

	if (!ft_check_ocp(ocp, 16) || param[0] >= 16)
		return (invalid(process));
	i = 0;
	while (i < REG_SIZE)
	{
		c = (process->registre[param[0]][i] % 256);
		write(1, &c, 1);
		i++;
	}
	return (valid(process, ocp, 16));
}
