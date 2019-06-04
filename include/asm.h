/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asm.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <dde-jesu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/12 14:44:32 by prastoin          #+#    #+#             */
/*   Updated: 2019/06/04 18:09:47 by fbecerri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ASM_H
# define ASM_H

# include "common.h"
# include <unistd.h>
# include <stdint.h>
# include <stdbool.h>

# define IN 0
# define OUT 1
# define ARGS_MSG "Convert asm to corewar bytecode"
# define FLAG_S_MSG "Turn on streaming reading mode"
# define BUFFER_SIZE  4096
# define HEADER_SIZE (16 + PROG_NAME_LENGTH + COMMENT_LENGTH)
# define EXT ".cor"

# define FIX_SIZE_ERR "Program too big (Exceed CHAMP_MAX_SIZE)"

# define SEPARATOR "│"

# define MSG "Expected on of "
# define SEV_ERROR CSI_RED "error: "
# define SEV_WARNING CSI_YELLOW "warning: "

typedef struct	s_flag
{
	bool	streaming;
	bool	werror;
}				t_flag;

typedef enum	e_severity
{
	Warn,
	Err
}				t_severity;

typedef struct	s_header
{
	char	name[PROG_NAME_LENGTH];
	char	comment[COMMENT_LENGTH];
	size_t	size;
}				t_header;

typedef struct	s_label
{
	uint8_t *name;
	size_t	offset;
}				t_label;


typedef union	u_param
{
	t_core_param		type;
	t_param_offset		offset;
	t_param_register	reg;
}				t_param;

typedef struct	s_instruction
{
	char	*label;
	ssize_t	opcode;
	t_param	params[MAX_PARAM];
}				t_instruction;

typedef struct	s_pos
{
	size_t	param;
	size_t	offset;
	uint8_t	size;
}				t_pos;

typedef struct	s_pos_vec
{
	size_t	len;
	size_t	capacity;
	t_pos	inner[];
}				t_pos_vec;

typedef struct	s_entry
{
	char		*key;
	uint64_t	hash;
	uint64_t	offset;
	t_pos_vec	*positions;
}				t_entry;

typedef struct	s_hashtable
{
	size_t		size;
	t_entry		bucket[];
}				t_hashtable;

typedef struct	s_span
{
	size_t		offset;
	char		*file_name;
	uintmax_t	lines;
	uintmax_t	col;
}				t_span;

typedef struct	s_read
{
	uint8_t		buffer[BUFFER_SIZE];
	size_t		len;
	size_t		index;
	size_t		nbr_read;
	int			fd;
	bool		write_able;
	bool		werror;
	t_span		span;
	t_span		begin;
}				t_read;

t_write			init_write(void);
t_read			init_read(int fd, char *argv, bool werror);
void			ft_itoa_base(uintmax_t nb, char *str, uint8_t b,
		const char *base);
void			asm_header(t_write *out, t_read *in);
char			*from_int_to_type(size_t type);
void			ft_itoa_hexa(char *str, uintmax_t nb, size_t len);

/*
** bin
*/
void			bin_write_inst(t_write *out, t_instruction *inst);
void			bin_write_end(t_write *out);
void			bin_resolve_positions(t_write *out, t_pos_vec *positions);
bool			bin_write_header(t_header head, t_write *out);
void			bin_padding_ocp(uint8_t ocp, t_write *out, int8_t *size,
		uint8_t opcode);

/*
** error
*/
typedef struct	s_error
{
	t_span		*begin;
	t_span		*end;
	t_severity	*severity;
}				t_error;

void			print_error(t_read *in, t_severity severity, char *error,
		char *expected);
int				print_small_error(t_read *in, t_severity severity, char *error,
		char *expected);
void			mark_span(t_read *in);
void			error_msg(t_write *err, char *error);
void			error_severity(t_write *err, t_severity severity);
uint16_t		get_columns(int fd);
void			locate_error(t_write *err, t_span begin);
void			error_contxt_print(t_write *err, t_error pack, size_t i);

/*
** io
*/
void			io_write(t_write *out, void *o_data, size_t size);
void			io_write_number(t_write *w, uintmax_t n);
void			io_write_one(t_write *out, char c);
void			io_flush(t_write *out);
void			io_write_int(t_write *out, uintmax_t nb, size_t nb_bytes);
int32_t			io_readnum(t_read *rd);
ssize_t			io_fill(t_read *rd);
ssize_t			io_read(t_read *rd, uint8_t data[], size_t data_len);
void			io_next(t_read *rd);
bool			io_expect(t_read *rd, const char *str);
int16_t			io_peek(t_read *rd);
bool			io_skip(t_read *rd, char e);
bool			io_skip_until(t_read *rd, char *chars);
void			io_seek(t_write *out, ssize_t offset, bool set_cur);
void			io_write_read(t_write *out, uint8_t *tmp, size_t size);
/*
** hashtable.c
*/
t_entry			create_entry(char *key);
t_hashtable		*create_hashtable(size_t size);
t_entry			*insert_hashtable(t_hashtable **table, t_entry entry);
t_entry			*hashtable_get(t_hashtable *table, char *name);

/*
** vec.c
*/
t_pos_vec		*create_pos_vec(size_t capacity);
t_pos			*add_position(t_pos_vec **vec);

/*
** asm_labels2.c
*/
void		fill_pos(t_pos *pos, t_write *out, t_instruction *inst, size_t i);
bool	free_inst_label(t_instruction *inst, size_t i);
bool	check_entry_position(t_entry **entry, t_pos **pos);
void	fix_new_offset(t_entry *entry, t_instruction *inst,
		size_t i, t_write *out);
size_t	size_until_param(t_instruction *inst, size_t pos);


/*
** asm_utils.c
*/
bool			asm_skip_ws(t_read *rd);
bool			asm_read_quoted(t_read *rd, char data[], size_t len);
void			asm_read_offset_value(t_read *in, t_param *param);
ssize_t			asm_opcode_for(char *name);

/*
** asm_parse.c
*/
t_header		asm_read_header(t_read *in);
char			*asm_get_inst(t_read *in);
bool			asm_read_params(t_read *in, t_instruction *inst);

/*
** argv_management.c
*/
bool			parse_long(const t_arg opt[], char **arg, char *argv[],
		size_t *i);
bool			parse_short(const t_arg opt[], char **arg, char *argv[],
		size_t *i);
ssize_t			parse_args(const t_arg args[], int argc, char *argv[]);

/*
** argv_management.c
*/

bool			asm_read_inst(t_read *in, t_instruction *inst);
bool			asm_transform(t_write *out, t_read *in);
bool			asm_store_label(t_hashtable **table, char *label, t_write *out,
		t_read *in);
bool			asm_resolve_label(t_hashtable **table, t_instruction *inst,
		t_write *out, t_read *in);
void			asm_check_labels(t_hashtable *table, t_read *in);

/*
** others
*/
bool			escape(t_read *rd, int16_t c);

#endif
