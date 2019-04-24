/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fcnt_09_12.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/04 13:27:13 by prastoin          #+#    #+#             */
/*   Updated: 2019/04/24 11:05:25 by prastoin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

bool		zjmp(t_vm *game, t_process *process, int32_t param[4], uint8_t ocp)
{
	ssize_t	offset;
	(void)ocp;
	(void)game;
	dprintf(g_fd, "P%5d | zjmp %d %s\n", g_opc, param[0], process->carry == true ? "OK" : "FAILED");
	if (process->carry == false)
		return (invalid(process, 0b11000000, 9));
	offset = (process->offset + /*(int32_t)*/param[0] % IDX_MOD);
	while (offset < 0)
		offset += MEM_SIZE;
	process->offset = offset % MEM_SIZE;
	return (true);
}

bool		ldi(t_vm *game, t_process *process, int32_t param[4], uint8_t ocp)
{
	uint8_t		op1[REG_SIZE];
	uint8_t		adr[REG_SIZE];
	int32_t	adress;

	if (param[2] > 16 || param[0] <= 0)
		return (invalid(process, ocp, 10));
	if (!ft_get_value_mod(param[0], (ocp >> 6 & 0b11), process, game))
		return (invalid(process, ocp, 10));
	ft_memcpy(op1, process->tampon, REG_SIZE);
	if (!ft_get_value_mod(param[1], (ocp >> 4 & 0b11), process, game))
		return (invalid(process, ocp, 10));
	bin_add(op1, process->tampon, adr);
	adress = (conv_bin_num(adr, REG_SIZE) % IDX_MOD);
	mem_read(game->mem, process->registre[param[2] - 1], process->offset + adress, REG_SIZE);
	while (adress + process->offset < 0)
		adress += MEM_SIZE;
	dprintf(g_fd, "P%5d | ldi %ld %ld r%d\n", g_opc, conv_bin_num(op1, REG_SIZE), conv_bin_num(process->tampon, REG_SIZE), param[2]);
	dprintf(g_fd, "       | -> load from %ld + %ld = %ld (with pc and mod %ld)\n", conv_bin_num(op1, REG_SIZE), conv_bin_num(process->tampon, REG_SIZE), conv_bin_num(adr, REG_SIZE), (process->offset + adress) % MEM_SIZE);
	return (valid(process, ocp, 10));
}

bool		sti(t_vm *game, t_process *process, int32_t param[4], uint8_t ocp)
{
	uint8_t		op1[REG_SIZE];
	uint8_t		adr[REG_SIZE];
	intmax_t	adress;

	if (param[0] > 16 || param[0] <= 0)
		return (invalid(process, ocp, 11));
	if (!ft_get_value_mod(param[1], (ocp >> 4 & 0b11), process, game))
		return (invalid(process, ocp, 11));
	ft_memcpy(op1, process->tampon, REG_SIZE);
	if (!ft_get_value(param[2], (ocp >> 2 & 0b11), process, game))
		return (invalid(process, ocp, 11));
	bin_add(op1, process->tampon, adr);
	adress = (conv_bin_num(adr, REG_SIZE)) % IDX_MOD;
	mem_write(game->mem, process->registre[param[0] - 1], process->offset + adress, REG_SIZE);
	dprintf(g_fd, "P%5d | sti r%ld %ld %d\n", g_opc, param[0], conv_bin_num(op1, REG_SIZE), conv_bin_num(process->tampon, REG_SIZE));
	dprintf(g_fd, "       | -> store to %ld + %ld = %ld (with pc and mod %ld)\n", conv_bin_num(op1, REG_SIZE), conv_bin_num(process->tampon, REG_SIZE), conv_bin_num(adr, REG_SIZE), process->offset + adress);
	return (valid(process, ocp, 11));
}

bool		ft_fork(t_vm *game, t_process *process, int32_t param[4], uint8_t ocp)
{
	t_process	*new_process;
	size_t		index;
	int			save;

	save = param[0];
	param[0] = param[0] % IDX_MOD;
	while (param[0] + process->offset < 0)
		param[0] += MEM_SIZE;
	index = (process - game->vec->processes);
	new_process = add_process(&game->vec);
	process = game->vec->processes + index;
	*new_process = (t_process) {
		.offset = (process->offset + param[0]) % MEM_SIZE, // TODO: Negative offset
		.is_alive = true,
	};
	copy_process(new_process, process);
	dprintf(g_fd, "P%5d | fork %d (%d)\n", g_opc, save, (save % IDX_MOD) + process->offset);
	return (valid(process, 0b11000000, 12));
}
