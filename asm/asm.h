/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asm.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/12 14:44:32 by prastoin          #+#    #+#             */
/*   Updated: 2019/03/13 17:14:56 by prastoin         ###   ########.fr       */
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

#include "op.h"

#define BUFFER_SIZE 4096
#define HEADER_SIZE 2192

typedef struct	s_read
{
	uint8_t		buffer[BUFFER_SIZE];
	size_t		len;
	size_t		index;
	size_t		nbr_read;
	size_t		fd;
	size_t		fd2;
}				t_read;

ssize_t		io_moveto(t_read *rd, uint8_t c);
void	*ft_memcpy(void *dst, const void *src, size_t n);
ssize_t		io_read(t_read *rd, uint8_t data[], size_t data_len);
t_read		init_read(size_t fd, size_t fd2);
ssize_t		io_fill(t_read *rd);
void	ft_itoa_base(uintmax_t nb, char *str, uint8_t b, const char *base);
ssize_t		header(t_read *rd);

#endif
