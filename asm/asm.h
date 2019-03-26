/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asm.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/12 14:44:32 by prastoin          #+#    #+#             */
/*   Updated: 2019/03/26 16:31:50 by prastoin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ASM_H
#define ASM_H

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include <stdbool.h>

#include "op.h"

#define BUFFER_SIZE  4096
#define HEADER_SIZE  16 + PROG_NAME_LENGTH + COMMENT_LENGTH
#define EXT ".cor"

#define MAX_PARAM 4

typedef enum e_severity{
	SEVERITY_WARNING,
	SEVERITY_ERROR
}	t_severity;

typedef enum e_core_param {
	PARAM_NONE = 0b0,
	PARAM_DIRECT = 0b1,
	PARAM_INDIRECT = 0b10,
	PARAM_REGISTER = 0b100,
	PARAM_ALL = 0b111,
	PARAM_INDEX = 0b1000
}	t_core_param;

typedef struct	s_core_tab
{
	size_t			opcode;
	char			*name;
	t_core_param	params[MAX_PARAM];
}				t_core_tab;


typedef struct	s_header
{
	char	name[PROG_NAME_LENGTH];
	char	comment[COMMENT_LENGTH];
	size_t	size;
}				t_header;

typedef enum e_param_type {
	TYPE_LABEL_INDIRECT,
	TYPE_LABEL_DIRECT,
	TYPE_OFFSET_DIRECT,
	TYPE_OFFSET_INDIRECT,
	TYPE_REGISTER
}			t_param_type;

typedef struct	s_param_offset {
	t_core_param	type;
	char			*label;
	int32_t			offset;
}				t_param_offset;

typedef struct	s_param_register {
	t_core_param	type;
	uint8_t			reg;
}				t_param_register;

typedef union	u_param {
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

typedef struct	s_label
{
	uint8_t *name;
	size_t	offset;
}				t_label;

typedef struct	s_write
{
	uint8_t		*buffer;
	size_t		index;
	size_t		nbr_write;
	bool		flushable;
	size_t		fd;
	size_t		buffer_size;
}				t_write;

typedef struct		s_entry{
	char		*key;
	uint64_t	hash;
	uint64_t	offset;
	bool		resolve;
}					t_entry;

typedef struct	s_hashtable{
	size_t		size;
	t_entry		bucket[];
}				t_hashtable;

typedef struct	s_span{
	size_t		offset;
	char		*file_name;
	uintmax_t		lines;
	uintmax_t		col;
}				t_span;

typedef struct	s_read
{
	uint8_t		buffer[BUFFER_SIZE];
	size_t		len;
	size_t		index;
	size_t		nbr_read;
	int			fd;
	t_span		span;
	t_span		begin;
}				t_read;

#include <stdio.h>

extern t_core_tab g_ops[17];

void		io_next(t_read *rd);
t_write		init_write(void);
void	*ft_memcpy(void *dst, const void *src, size_t n);
ssize_t		io_read(t_read *rd, uint8_t data[], size_t data_len);
t_read		init_read(int fd, char *argv);
ssize_t		io_fill(t_read *rd);
void	ft_itoa_base(uintmax_t nb, char *str, uint8_t b, const char *base);
ssize_t		header(t_read *rd);
bool		io_expect(t_read *rd, const char *str);
int16_t		io_peek(t_read *rd);
bool		io_skip(t_read *rd, char e);
bool		io_skip_until(t_read *rd, char *chars);
int32_t	io_readnum(t_read *rd);
bool	write_header(t_header *head, t_write *out);
void		bin_write_inst(t_write *out, t_instruction *inst, uint8_t last_label);
void	io_write_int(t_write *out, uintmax_t nb, size_t nb_bytes);
void	bin_write_end(t_write *out);
void	io_flush(t_write *out);
void		bin_resolve_label(t_write *out, size_t offset);
void	print_error(uintmax_t severity, t_span begin, t_span end, char *error, char *expected);
char	*from_int_to_type(size_t type);

/*
** Hashtable.c
*/
t_entry			create_entry(char *key);
t_hashtable		*create_hashtable(size_t size);
t_entry			*insert_hashtable(t_hashtable **table, t_entry entry);
t_entry	*hashtable_get(t_hashtable *table, char *name);

#endif
