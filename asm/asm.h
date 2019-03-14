/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asm.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/12 14:44:32 by prastoin          #+#    #+#             */
/*   Updated: 2019/03/14 18:11:47 by prastoin         ###   ########.fr       */
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
#include <stdbool.h>

#include "op.h"

#define BUFFER_SIZE  4096
#define HEADER_SIZE  2192

#define MAX_PARAM 4

typedef enum e_core_param {
	PARAM_NONE = 0b0,
	PARAM_DIRECT = 0b1,
	PARAM_INDIRECT = 0b10,
	PARAM_REGISTER = 0b100,
	PARAM_ALL = 0b111,
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

typedef struct	s_param_direct {
	t_core_param	type;
	char			*label;
	int32_t			offset;
}				t_param_direct;

typedef struct	s_param_indirect {
	t_core_param	type;
	char			*label;
	int32_t			offset;
}				t_param_indirect;

typedef struct	s_param_register {
	t_core_param	type;
	uint8_t			reg;
}				t_param_register;

typedef union	u_param {
	t_core_param		type;
	t_param_direct		direct;
	t_param_indirect	indirect;
	t_param_register	reg;
}				t_param;

typedef struct	s_instruction
{
	char	*label;
	size_t	opcode;
	t_param	params[MAX_PARAM];
}				t_instruction;

typedef struct	s_write
{
	uint8_t		buffer[BUFFER_SIZE];
	size_t		index;
	size_t		len;
}				t_write;

typedef struct	s_read
{
	uint8_t		buffer[BUFFER_SIZE];
	size_t		len;
	size_t		index;
	size_t		nbr_read;
	int			fd;
}				t_read;

#include <stdio.h>

extern t_core_tab g_ops[17]; //IIIJOICIIIICIIIIC

void	*ft_memcpy(void *dst, const void *src, size_t n);
ssize_t		io_read(t_read *rd, uint8_t data[], size_t data_len);
t_read		init_read(int fd);
ssize_t		io_fill(t_read *rd);
void	ft_itoa_base(uintmax_t nb, char *str, uint8_t b, const char *base);
ssize_t		header(t_read *rd);
bool		io_expect(t_read *rd, const char *str);
int16_t		io_peek(t_read *rd);
bool		io_skip(t_read *rd, char e);
uint32_t	io_readnum(t_read *rd);

#endif
