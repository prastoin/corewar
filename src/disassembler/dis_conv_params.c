/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pass_opcode.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/05 13:47:57 by prastoin          #+#    #+#             */
/*   Updated: 2019/05/22 22:59:55 by fbecerri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "disassembler.h"

void		diss_get_params_no_ocp(const uint8_t mem[CHAMP_MAX_SIZE],
		t_read_prog *prog, t_diss diss)
{
	const int32_t	opcode = prog->opcode;
	size_t			size;
	uint8_t			stck[4];

	size = 0;
	if (g_ops[opcode].params[0] & Param_Register)
	{
		ft_putf_fd(diss.fd_out, "r");
		size = 1;
	}
	else if (g_ops[opcode].params[0] & Param_Direct)
	{
		ft_putf_fd(diss.fd_out, "%");
		size = g_ops[opcode].params[0] & Param_Index ? 2 : 4;
	}
	else if (g_ops[opcode].params[0] & Param_Indirect)
		size = 2;
	diss_mem_read(mem, stck, prog->offset + 1, size);
	ft_putf_fd(diss.fd_out, "%d\n", conv_bin(stck, size));
}

bool		diss_check_ocp(uint8_t ocp, uint8_t opcode)
{
	size_t i;
	size_t type;

	i = 0;
	while (g_ops[opcode].params[i])
	{
		type = (ocp >> (3 - i) * 2) & 0b11;
		if (type == 0b11)
			if (!(g_ops[opcode].params[i] & 0b10))
				return (false);
		if (type == 0b10)
			if (!(g_ops[opcode].params[i] & 0b1))
				return (false);
		if (type == 0b01)
			if (!(g_ops[opcode].params[i] & 0b100))
				return (false);
		if (type == 0b00)
			return (false);
		i++;
	}
	return (true);
}

bool		diss_read_params(const uint8_t mem[CHAMP_MAX_SIZE],
		t_read_prog *prog, t_diss diss)
{
	const int	opcode = prog->opcode;
	uint8_t		tampom[1];

	if (g_ops[opcode].ocp == true)
	{
		diss_mem_read(mem, tampom, prog->offset + 1, 1);
		prog->ocp = tampom[0];
		if (!(diss_check_ocp(prog->ocp, opcode)))
			return (false);
		ft_putf_fd(diss.fd_out, "	%s%s", g_ops[opcode].name,
			&"        "[ft_strlen(g_ops[opcode].name)]);
		diss_get_params_ocp(mem, prog, diss);
		prog->offset += diss_get_decale(prog->ocp, opcode);
	}
	else
	{
		ft_putf_fd(diss.fd_out, "	%s%s", g_ops[opcode].name,
			&"        "[ft_strlen(g_ops[opcode].name)]);
		diss_get_params_no_ocp(mem, prog, diss);
		prog->offset += diss_get_decale(opcode == 1 ? 0b10000000 : 0b11000000,
				opcode);
	}
	return (true);
}

bool		diss_pass(const uint8_t mem[CHAMP_MAX_SIZE],
		t_read_prog *prog, t_diss diss)
{
	if (!(diss_read_params(mem, prog, diss)))
		return (false);
	return (true);
}

bool		diss_read_opcode(const uint8_t mem[CHAMP_MAX_SIZE],
		t_read_prog *prog)
{
	uint8_t		stck[1];

	diss_mem_read(mem, stck, prog->offset, 1);
	prog->opcode = stck[0];
	if (prog->opcode <= 0 || prog->opcode > 16)
		return (false);
	return (true);
}
