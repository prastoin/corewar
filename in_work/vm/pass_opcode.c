/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pass_opcode.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/05 13:47:57 by prastoin          #+#    #+#             */
/*   Updated: 2019/04/08 16:19:22 by prastoin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

void	get_params_ocp(t_vm *vm, t_process *process, uint8_t ocp, size_t params[])
{
	const int	opcode = process->actual_opcode;
	size_t		i;
	size_t		size;
	uint8_t		stck[4];

	i = 0;
	size = 0;
	while (g_ops[opcode].params[i])
	{
		if (ocp >> ((3 - i) *  2) & 0b01)
			size = 1;
		else if (ocp >> ((3 - i) *  2) & 0b10)
			size = g_ops[opcode].params[i] & PARAM_INDEX ? 2 : 4;
		else if (ocp >> ((3 - i) *  2) & 0b11)
			size = 2;
		mem_read(vm->mem, stck, process->offset, size);
		params[i] = conv_bin_num(stck, size);
//		inc_process_off_mod(process, size, false); DAVID NEED TO WORKS TODO
		i++;
	}
}

void	get_params_no_ocp(t_vm *vm, t_process *process, size_t opcode, size_t params[])
{
	size_t	size;
	uint8_t	stck[4];

	size = 0;
	if (g_ops[opcode].params[0] & PARAM_REGISTER)
		size = 1;
	else if (g_ops[opcode].params[0] & PARAM_DIRECT)
		size = g_ops[opcode].params[0] & PARAM_INDEX ? 2 : 4;
	else if (g_ops[opcode].params[0] & PARAM_INDIRECT)
		size = 2;
	mem_read(vm->mem, stck, process->offset, size);
	params[0] = conv_bin_num(stck, size);
//	inc_process_off_mod(process, size, false); DAVID NEED TO WORKS TODO
}

bool	read_params_and_ocp(t_vm *vm, t_process *process, size_t params[4], uint8_t *ocp)
{
	const int	opcode = process->actual_opcode;
	uint8_t		tampom[1];

	if (g_ops[opcode].ocp == true)
	{
		mem_read(vm->mem, tampom, process->offset, 1);
		inc_process_off_mod(process, 1, false);
		*ocp = tampom[0];
		if (!(ft_check_ocp(*ocp, opcode)))
			return (false);
		get_params_ocp(vm, process, *ocp, params);
	}
	else
		get_params_no_ocp(vm, process, opcode, params);
	return (true);
}

bool	ft_pass(t_vm *vm, t_process *process)
{
	uint8_t ocp;
	size_t params[4];

	process->has_read = false;
	printf("\033[37;01m└─Read_params_and_ocp\033[0m\n");
	if (!(read_params_and_ocp(vm, process, params, &ocp)))
	{
		printf ("	\033[31m└─ERROR bad ocp %d \033[0m\n", ocp);
		return (false);
	}
	printf ("\033[32;01m	└─Executing the instruction %s \033[0m", g_ops[process->actual_opcode].name);
	g_fcnt[process->actual_opcode](vm, process, params, ocp);
	process->cycle_to_do = 0;
	return (true);
}

bool	read_opcode(t_vm *game, t_process *process)
{
	uint8_t stck[1];

	mem_read(game->mem, stck, process->offset, 1);
	inc_process_off_mod(process, 1, false);
	process->actual_opcode = stck[0];
	if (process->actual_opcode <= 0 || process->actual_opcode > 16)
	{
		printf ("\033[31m└─Bad opcode %d\033[0m\n", process->actual_opcode);
		process->actual_opcode = 0;
		return (false);
	}
	process->has_read = true;
	process->cycle_to_do = g_ops[stck[0]].cycle;
	printf ("\033[32;01m└─Process actual_opcode %d\033[0m\n", process->actual_opcode);
	return (true);
}
