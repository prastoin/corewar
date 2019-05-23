/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display_error.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/04 14:58:16 by prastoin          #+#    #+#             */
/*   Updated: 2019/05/23 11:41:50 by dde-jesu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"
#include <fcntl.h>
#include "ft_string.h"
#include <sys/stat.h>

void	locate_error(t_write *error, t_span begin)
{
	io_write(error, CSI_RESET, (sizeof(CSI_RESET) - 1));
	io_write(error, "\n- <", ft_strlen("\n- <"));
	io_write(error, begin.file_name, ft_strlen(begin.file_name));
	io_write(error, ">:", ft_strlen(">:"));
	io_write_number(error, begin.lines);
	io_write(error, ":", 1);
	io_write_number(error, begin.col);
	io_write(error, "\n", 1);
}

void	underline_error(t_write *error, t_error pack, char *line,
		size_t line_len)
{
	size_t i;

	i = 0;
	while (i < pack.begin->col - 1 && i < line_len)
	{
		if (line[i] == '\t')
			io_write(error, "    ", 4);
		else
			io_write(error, " ", 1);
		i++;
	}
	if (*(pack.severity) == Err)
		io_write(error, CSI_RED, (sizeof(CSI_RED) - 1));
	else
		io_write(error, CSI_YELLOW, (sizeof(CSI_YELLOW) - 1));
	io_write(error, "^", 1);
	i++;
	while (i < pack.end->col - 1 && i < line_len)
	{
		io_write(error, "^", 1);
		i++;
	}
}

void	print_first(t_write *error, t_span *begin)
{
	io_write(error, CSI_BLUE, sizeof(CSI_BLUE) - 1);
	io_write(error, "    ", 4 - nb_len(begin->lines % 1000));
	io_write_number(error, begin->lines % 1000);
	io_write(error, " " SEPARATOR " ", 2 + sizeof(SEPARATOR) - 1);
	io_write(error, CSI_RESET, (sizeof(CSI_RESET) - 1));
}

void	print_second(t_write *error, t_error pack, char buffer[4096],
		size_t len)
{
	char	*last;
	char	*tab;
	char	*b_end;

	last = buffer;
	b_end = buffer + len;
	while ((tab = ft_memchr(last, '\t', b_end - last)))
	{
		io_write(error, last, tab - last);
		io_write(error, "    ", 4);
		last = tab + 1;
	}
	io_write(error, last, b_end - last);
	io_write(error, "\n", 1);
	io_write(error, CSI_BLUE, sizeof(CSI_BLUE) - 1);
	io_write(error, "    ", 4);
	io_write(error, " " SEPARATOR " ", 2 + sizeof(SEPARATOR) - 1);
	underline_error(error, pack, buffer, len);
}

void	error_contxt_print(t_write *error, t_error pack, size_t i)
{
	int16_t			columns;
	int				fd;
	size_t			len;
	char			buffer[4096];
	struct stat		stats;

	i = 0;
	fd = open(pack.begin->file_name, O_RDONLY);
	if (fstat(fd, &stats) == -1 || !S_ISREG(stats.st_mode))
		return ;
	lseek(fd, pack.begin->offset - pack.begin->col + 1, SEEK_SET);
	len = pack.end->col + 100 > sizeof(buffer)
		? sizeof(buffer) : pack.end->col + 100;
	len = read(fd, buffer, len);
	columns = get_columns(STDERR_FILENO) - 6;
	while (i < len && buffer[i] != '\n' && columns > 1)
		if (buffer[i++] == '\t')
			columns -= 4;
		else
			columns--;
	len = i;
	print_first(error, pack.begin);
	print_second(error, pack, buffer, len);
}
