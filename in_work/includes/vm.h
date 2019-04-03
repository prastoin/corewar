/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vm.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/28 09:48:27 by prastoin          #+#    #+#             */
/*   Updated: 2019/04/03 09:09:30 by fbecerri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VM_H
#define VM_H

#include "op.h"
#include "asm.h"

typedef enum	e_arg_type
{
	ARG_END = 0,
	ARG_STRUCT,
	ARG_BOOLEAN,
	ARG_INT
}				t_type;

typedef struct	s_datan
{
	size_t	num;
	char	*name;
}				t_datan;

typedef struct	s_core_fcnt
{
	size_t			opcode;
	void (*fcnt)(void *, void *, void *, void *);
}				t_core_fcnt;

typedef	struct	s_arg
{
	t_type	type;
	uint8_t	short_name;
	char	*long_name;
	void	*value;
	const char *help;
}				t_arg;

typedef struct	s_flags
{
	t_datan	*num;
	size_t	dump_c;
	size_t	run_c;
	bool	bin_o;
	bool	ncurse_o;
}				t_flags;

typedef struct	s_process
{
	size_t		offset;
	size_t		cycle;
	uint16_t	registre[16];
	uint8_t		opcode;
//	void(*fcnt)(int tab[4], int ocp);
}				t_process;

typedef struct	s_vec
{
	size_t		len;
	size_t		capacity;
	t_process	processes[];
}				t_vec;

typedef struct	s_champ
{
	char		*name;
	size_t		fd;
	size_t		nbr_champ;
	int			num;
	t_header	header;
	t_vec		*processes;
}				t_champ;

#endif
