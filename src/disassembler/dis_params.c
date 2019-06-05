/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dis_params.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbecerri <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/22 22:52:32 by fbecerri          #+#    #+#             */
/*   Updated: 2019/06/05 11:32:07 by prastoin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "disassembler.h"

bool		error_message(const char *err)
{
	ft_putf_fd(2, "%serror : %s%s.\n", CSI_RED, err, CSI_RESET);
	return (false);
}

size_t		diss_get_decale(uint8_t ocp, int opcode)
{
	size_t size;
	size_t type;
	size_t i;

	size = g_ops[opcode].ocp ? 2 : 1;
	i = 0;
	while (g_ops[opcode].params[i])
	{
		type = (ocp >> ((3 - i) * 2)) & 0b11;
		if (type == 0b01)
			size += 1;
		else if (type == 0b10)
			size += g_ops[opcode].params[i] & Param_Index ? 2 : 4;
		else if (type == 0b11)
			size += 2;
		i++;
	}
	return (size);
}

void		diss_mem_read(const uint8_t mem[CHAMP_MAX_SIZE], uint8_t *dst,
		ssize_t offset, size_t n)
{
	ssize_t i;

	while (offset < 0)
		offset += CHAMP_MAX_SIZE;
	i = n - 1;
	while (i >= 0)
	{
		dst[i] = mem[(offset + i) % CHAMP_MAX_SIZE];
		i--;
	}
}

void		aff_atribute_ocp(t_diss diss, uint32_t ocp, size_t i)
{
	if ((ocp >> ((3 - i) * 2) & 0b11) == 0b01)
		ft_putf_fd(diss.fd_out, "r");
	else if ((ocp >> ((3 - i) * 2) & 0b11) == 0b10)
		ft_putf_fd(diss.fd_out, "%");
}

void		diss_get_params_ocp(const uint8_t mem[CHAMP_MAX_SIZE],
		t_read_prog *prog, t_diss diss)
{
	const uint32_t	ocp = prog->ocp;
	size_t			i;
	size_t			size;
	uint8_t			stck[4];
	size_t			tmp;

	i = 0;
	size = 0;
	tmp = 2;
	while (g_ops[prog->opcode].params[i])
	{
		aff_atribute_ocp(diss, ocp, i);
		if ((ocp >> ((3 - i) * 2) & 0b11) == 0b01)
			size = 1;
		else if ((ocp >> ((3 - i) * 2) & 0b11) == 0b10)
			size = g_ops[prog->opcode].params[i] & Param_Index ? 2 : 4;
		else if ((ocp >> ((3 - i) * 2) & 0b11) == 0b11)
			size = 2;
		diss_mem_read(mem, stck, prog->offset + tmp, size);
		ft_putf_fd(diss.fd_out, "%d", conv_bin(stck, size));
		tmp += size;
		i++;
		g_ops[prog->opcode].params[i] ? ft_putf_fd(diss.fd_out, ", ")
			: ft_putf_fd(diss.fd_out, "\n");
	}
}
