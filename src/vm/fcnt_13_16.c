/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fcnt_13_16.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/04 13:31:01 by prastoin          #+#    #+#             */
/*   Updated: 2019/04/22 16:18:02 by prastoin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

bool		lld(t_vm *game, t_process *process, int32_t param[4], uint8_t ocp)
{
	if (param[1] >= 16 || param[1] < 0)
		return (carry_down(process, ocp, 13));
	if (!ft_get_value(param[0], ocp >> 6 & 0b11, process, game))
		return (carry_down(process, ocp, 13));
	ft_memcpy(process->registre[param[1]], process->tampon, REG_SIZE);
	return (carry_up(process, ocp, 13));
}

bool		lldi(t_vm *game, t_process *process, int32_t param[4], uint8_t ocp)
{
	uint8_t		op1[REG_SIZE];
	uint8_t		adr[REG_SIZE];
	uint64_t	adress;

	if (param[2] >= 16 || param[2] < 0)
		return (carry_down(process, ocp, 14));
	if (!ft_get_value(param[0], (ocp >> 6 & 0b11), process, game))
		return (carry_down(process, ocp, 14));
	ft_memcpy(op1, process->tampon, REG_SIZE);
	if (!ft_get_value(param[1], (ocp >> 4 & 0b11), process, game))
		return (carry_down(process, ocp, 14));
	bin_add(op1, process->tampon, adr);
	adress = (conv_bin_num(adr, REG_SIZE));
	while ((process->offset + adress) < 0)
		adress += MEM_SIZE;
	mem_read(game->mem, process->registre[param[2]], (process->offset + adress) % MEM_SIZE, REG_SIZE);
	return (carry_up(process, ocp, 14));
}

bool		lfork(t_vm *game, t_process *process, int32_t param[4], uint8_t ocp)
{
	t_process	*new_process;
	size_t		index;

	while ()
	index = (process - game->vec->processes);
	new_process = add_process(&game->vec);
	process = game->vec->processes + index;
	*new_process = (t_process) {
		.offset = (process->offset + param[1]) % MEM_SIZE,
		.is_alive = true
	};
	copy_process(new_process, process);
	return (valid(process, 0b11, 15));
}

bool		aff(t_vm *game, t_process *process, int32_t param[4], uint8_t ocp)
{
	size_t		i;
	uint8_t		c;

	(void)game;
	if (param[0] >= 16 || param[0] < 0)
		return (invalid(process, ocp, 16));
	i = 0;
	while (i < REG_SIZE)
	{
		c = (process->registre[param[0]][i] % 256);
		write(1, &c, 1);
		i++;
	}
	return (valid(process, ocp, 16));
}
