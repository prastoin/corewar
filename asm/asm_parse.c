/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asm_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/13 09:51:51 by prastoin          #+#    #+#             */
/*   Updated: 2019/03/13 12:30:01 by prastoin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"
#include "ft_string.h"
#include <stdio.h>

int		error(char *str)
{
	dprintf(2, "%s\n", str);
	return (-1);
}

ssize_t	check_arg(char *name, uint8_t cor[])
{
	size_t len;
	ssize_t fd;

	if ((len = ft_strlen(name)) > 127)
		return (error("Invalid name file : too long\n"));
	if (!(search_for((uint8_t *)name, '.') == 1 && name[len - 2] == '.' && name[len - 1] == 's'))
		return (error("Invalid name file : no .s\n"));
	ft_strcpy(cor, (uint8_t *)name);
	cor[len - 1] = 'c';
	cor[len] = 'o';
	cor[len + 1] = 'r';
	cor[len + 2] = '\0';
	if ((fd = open((const char *)cor, O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR)) < 0)
		return (error("Failed to create file .cor\n"));
	return (fd);
}

int		main(int argc, char **argv)
{
	ssize_t	fd;
	ssize_t	fd2;
	uint8_t	tmp[150];
	t_read	rd;

	if ((fd = open(argv[1], O_RDWR)) < 0)
		return (error("Failed to open .s file.\n"));
	if ((fd2 = check_arg(argv[1], tmp)) < 0)
		return (-1);
	rd = init_read(fd, fd2);
	return (0);
}
