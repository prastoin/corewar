/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asm.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/12 14:44:32 by prastoin          #+#    #+#             */
/*   Updated: 2019/03/14 12:28:51 by prastoin         ###   ########.fr       */
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

#define BUFFER_SIZE 4096
#define HEADER_SIZE 2192

typedef struct	s_header
{
	char	name[PROG_NAME_LENGTH];
	char	comment[COMMENT_LENGTH];
	size_t	size;
}				t_header;

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

void	*ft_memcpy(void *dst, const void *src, size_t n);
ssize_t		io_read(t_read *rd, uint8_t data[], size_t data_len);
t_read		init_read(int fd);
ssize_t		io_fill(t_read *rd);
void	ft_itoa_base(uintmax_t nb, char *str, uint8_t b, const char *base);
ssize_t		header(t_read *rd);
bool		io_expect(t_read *rd, const char *str);
int16_t		io_peek(t_read *rd);
bool		io_skip(t_read *rd, char e);

#endif
