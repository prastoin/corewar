/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asm_write.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/13 13:22:24 by prastoin          #+#    #+#             */
/*   Updated: 2019/03/13 14:24:24 by prastoin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"
#include "ft_string.h"

ssize_t		header(t_read *rd, uint8_t name[])
{
	uint8_t tmp[132];
	uint8_t str[8];
	size_t	i;

	i = 0;
	if ((io_read(rd, str, 5) < 0) || (ft_strncmp(str, ".name", 5) != 0))
		return (-1);
	if (io_moveto(rd, '"') < 0)
		return (-1);

	return (0);
}
