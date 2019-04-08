/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vm.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/28 09:48:27 by prastoin          #+#    #+#             */
/*   Updated: 2019/04/08 11:58:47 by prastoin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VM_H
#define VM_H

#include "op.h"
#include "asm.h"
#define OCP_DIR 0b10
#define OCP_REG 0b01
#define OCP_IND 0b11


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
	uint8_t actual_opcode;
	//au dessus need avis
	bool		carry;
	size_t		offset;
	bool		has_read;
	size_t		cycle_to_do;
	uint8_t		registre[16][REG_SIZE];
	uint8_t		tampon[REG_SIZE];
	bool		is_alive;
	bool		said_live;
}				t_process;

typedef struct	s_vec
{
	size_t		len;
	size_t		capacity;
	t_process	processes[];
}				t_vec;

typedef struct	s_champ
{
	char	name[PROG_NAME_LENGTH];
	char	comment[COMMENT_LENGTH];
	char	prog[CHAMP_MAX_SIZE];
	size_t	size;
	size_t	fd;
	size_t	last_cycle_live;
}				t_champ;

typedef struct	s_vm
{
	unsigned char		mem[MEM_SIZE];
	uintmax_t	cycle;
	uintmax_t	cycle_to_die;
	bool		live[MAX_PLAYERS];
	size_t		nbr_champ;
	size_t		nbr_live;
	bool		continu;
	size_t		check;
	t_champ		champ[MAX_PLAYERS];
	t_vec		*vec;
}				t_vm;




typedef	bool (*t_core_fcnt)(t_vm *, t_process *, size_t *, uint8_t);

extern t_core_fcnt g_fcnt[17];

bool	ft_check_ocp(uint8_t ocp, uint8_t opcode);
bool	ft_get_value(ssize_t nbr, uint8_t type, t_process *processes, t_vm *vm);
bool	ft_get_value_mod(ssize_t nbr, uint8_t type, t_process *processes, t_vm *vm);
void	mem_write(char mem[], const uint8_t *value, size_t offset, size_t size);
void	mem_read(const char mem[], uint8_t str[], size_t offset, size_t size);
void	mem_write_one(char mem[], uint8_t c, size_t offset);
void	mem_write_int(char mem[], size_t nb, size_t len, size_t offset);
bool	ft_play(t_vm vm);
t_vec	*create_process(size_t capacity);
t_process	add_process(t_vec **list, size_t offset);
bool		carry_down(t_process *process);
bool		carry_up(t_process *process, uint8_t ocp, int opcode);
bool		invalid(t_process *process);
bool		valid(t_process *process, uint8_t ocp, int opcode);
void		inc_process_off_mod(t_process *process, size_t size, bool mod);
bool		ft_pass(t_vm *vm, t_process *process);
bool		read_opcode(t_vm *vm, t_process *process);

/*
** bin_operand
*/

bool		bin_add(uint8_t op1[REG_SIZE], uint8_t op2[REG_SIZE], uint8_t res[REG_SIZE]);
bool		bin_sub(uint8_t op1[REG_SIZE], uint8_t op2[REG_SIZE], uint8_t res[REG_SIZE]);
void		bin_and(uint8_t op1[REG_SIZE], uint8_t op2[REG_SIZE], uint8_t res[REG_SIZE]);
void		bin_or(uint8_t op1[REG_SIZE], uint8_t op2[REG_SIZE], uint8_t res[REG_SIZE]);
void		bin_xor(uint8_t op1[REG_SIZE], uint8_t op2[REG_SIZE], uint8_t res[REG_SIZE]);
uint32_t	conv_bin_num(uint8_t *str, uint8_t len);
void		conv_int_to_bin(size_t nbr, uint8_t[REG_SIZE]);
void		copy_process(t_process *dest, t_process *src);

/*
** op.c
*/

bool	live(t_vm *game, t_process *process, size_t *param, uint8_t ocp);
bool	ld(t_vm *game, t_process *process, size_t *param, uint8_t ocp);
bool	st(t_vm *game, t_process *process, size_t *param, uint8_t ocp);
bool	add(t_vm *game, t_process *process, size_t *param, uint8_t ocp);
bool	sub(t_vm *game, t_process *process, size_t *param, uint8_t ocp);
bool	ft_and(t_vm *game, t_process *process, size_t *param, uint8_t ocp);
bool	ft_or(t_vm *game, t_process *process, size_t *param, uint8_t ocp);
bool	ft_xor(t_vm *game, t_process *process, size_t *param, uint8_t ocp);
bool	zjmp(t_vm *game, t_process *process, size_t *param, uint8_t ocp);
bool	ldi(t_vm *game, t_process *process, size_t *param, uint8_t ocp);
bool	sti(t_vm *game, t_process *process, size_t *param, uint8_t ocp);
bool	ft_fork(t_vm *game, t_process *process, size_t *param, uint8_t ocp);
bool	lld(t_vm *game, t_process *process, size_t *param, uint8_t ocp);
bool	lldi(t_vm *game, t_process *process, size_t *param, uint8_t ocp);
bool	lfork(t_vm *game, t_process *process, size_t *param, uint8_t ocp);
bool	aff(t_vm *game, t_process *process, size_t *param, uint8_t ocp);



#endif
