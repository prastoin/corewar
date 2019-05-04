/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display_error.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/04 14:58:16 by prastoin          #+#    #+#             */
/*   Updated: 2019/05/04 15:07:51 by prastoin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"
#include "ft_string.h"
#include <fcntl.h>

#define SEPARATOR "│"

void	locate_error(t_write *err, t_span begin)
{
	io_write(err, CSI_RESET, (sizeof(CSI_RESET) - 1));
	io_write(err, "\n- <", ft_strlen("\n- <"));
	io_write(err, begin.file_name, ft_strlen(begin.file_name));
	io_write(err, ">:", ft_strlen(">:"));
	io_write_number(err, begin.lines);
	io_write(err, ":", 1);
	io_write_number(err, begin.col);
	io_write(err, "\n", 1);
}

void	underline_error(t_write *err, t_error pack, char *line, size_t line_len)
{
	size_t i;

	i = 0;
	while (i < pack.begin->col - 1 && i < line_len)
	{
		if (line[i] == '\t')
			io_write(err, "    ", 4);
		else
			io_write(err, " ", 1);
		i++;
	}
	if (*(pack.severity) == ERR)
		io_write(err, CSI_RED, (sizeof(CSI_RED) - 1));
	else
		io_write(err, CSI_YELLOW, (sizeof(CSI_YELLOW) - 1));
	io_write(err, "^", 1);
	i++;
	while (i < pack.end->col - 1 && i < line_len)
	{
		io_write(err, "^", 1);
		i++;
	}
}

void	print_first(t_write *err, t_span *begin)
{
	io_write(err, CSI_BLUE, sizeof(CSI_BLUE) - 1);
	io_write(err, "    ", 4 - nb_len(begin->lines % 1000));
	io_write_number(err, begin->lines % 1000);
	io_write(err, " " SEPARATOR " ", 2 + sizeof(SEPARATOR) - 1);
	io_write(err, CSI_RESET, (sizeof(CSI_RESET) - 1));
}

void	print_second(t_write *err, t_error pack, char buffer[4096], size_t len)
{
	char	*last;
	char	*tab;
	char	*b_end;

	last = buffer;
	b_end = buffer + len;
	while ((tab = ft_memchr(last, '\t', b_end - last)))
	{
		io_write(err, last, tab - last);
		io_write(err, "    ", 4);
		last = tab + 1;
	}
	io_write(err, last, b_end - last);
	io_write(err, "\n", 1);
	io_write(err, CSI_BLUE, sizeof(CSI_BLUE) - 1);
	io_write(err, "    ", 4);
	io_write(err, " " SEPARATOR " ", 2 + sizeof(SEPARATOR) - 1);
	underline_error(err, pack, buffer, len);
}

void	error_contxt_print(t_write *err, t_error pack, size_t i)
{
	const uint16_t	columns = get_columns(2);
	size_t			fd;
	size_t			len;
	size_t			size;
	char			buffer[4096];

	i = 0;
	fd = open(pack.begin->file_name, O_RDONLY);
	lseek(fd, pack.begin->offset - pack.begin->col + 1, SEEK_SET);
	len = pack.end->col + 100 > sizeof(buffer)
		? sizeof(buffer) : pack.end->col + 100;
	len = read(fd, buffer, len);
	size = 6;
	while (i < len && buffer[i] != '\n' && size < (columns - 1))
	{
		if (buffer[i] == '\t')
			size += 4;
		else
			size++;
		i++;
	}
	len = i;
	print_first(err, pack.begin);
	print_second(err, pack, buffer, len);
}
