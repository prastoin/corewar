/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   disassembler.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbecerri <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/13 11:46:37 by fbecerri          #+#    #+#             */
/*   Updated: 2019/05/22 23:24:10 by prastoin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DISASSEMBLER_H

# define DISASSEMBLER_H

# include "common.h"
# include "ft_string.h"

typedef struct	s_diss
{
	int	fd_in;
	int	fd_out;
}				t_diss;

typedef struct	s_head
{
	char	name[PROG_NAME_LENGTH];
	char	comment[COMMENT_LENGTH];
	uint8_t	prog[CHAMP_MAX_SIZE];
	size_t	size;
}				t_head;

typedef struct	s_read_prog
{
	size_t		offset;
	uint32_t	opcode;
	uint32_t	ocp;
}				t_read_prog;

size_t			diss_get_decale(uint8_t ocp, int opcode);
void			diss_get_params_ocp(const uint8_t mem[CHAMP_MAX_SIZE],
		t_read_prog *prog, t_diss diss);
void			diss_mem_read(const uint8_t mem[CHAMP_MAX_SIZE], uint8_t *dst,
		ssize_t offset, size_t n);
intmax_t		conv_bin(uint8_t *mem, size_t len);
bool			diss_read_opcode(const uint8_t mem[CHAMP_MAX_SIZE],
		t_read_prog *prog);
bool			diss_pass(const uint8_t mem[CHAMP_MAX_SIZE], t_read_prog *prog,
		t_diss diss);

#endif
