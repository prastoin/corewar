/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/13 17:54:07 by prastoin          #+#    #+#             */
/*   Updated: 2019/04/29 10:47:52 by prastoin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_string.h"
#include "asm.h"
#include <fcntl.h>
#include <sys/ioctl.h>

#define MSG "Expected one of "
#define SEV_ERROR CSI_RED "error: "
#define SEV_WARNING CSI_YELLOW "warning: "
#define SEPARATOR "│"

char	*from_int_to_type(size_t type)
{
	type &= PARAM_ALL;
	if (type == PARAM_ALL)
		return (MSG "Direct | Registre | Indirect");
	if (type == (PARAM_DIRECT | PARAM_INDIRECT))
		return (MSG "Direct | Indirect");
	if (type == (PARAM_DIRECT | PARAM_REGISTER))
		return (MSG "Direct | Registre");
	if (type == (PARAM_INDIRECT | PARAM_REGISTER))
		return (MSG "Indirect | Registre");
	if (type == PARAM_DIRECT)
		return ("Expected Direct");
	if (type == PARAM_REGISTER)
		return ("Expected Registre");
	if (type == PARAM_INDIRECT)
		return ("Expected Indirect");
	return (NULL);
}

void	error_severity(t_write *err, size_t severity)
{
	if (severity == 1)
		io_write(err, SEV_ERROR, sizeof(SEV_ERROR) - 1);
	if (severity == 2)
		io_write(err, SEV_WARNING, sizeof(SEV_WARNING) - 1);
	io_write(err, CSI_RESET, sizeof(CSI_RESET) - 1);
}

void	error_msg(t_write *err, char *error)
{
	io_write(err, CSI_WHITE, sizeof(CSI_WHITE) - 1);
	io_write(err, error, ft_strlen(error));
}

void	io_write_number(t_write *w, uintmax_t n)
{
	uintmax_t	tmp;
	uintmax_t	pow;
	char		c;

	tmp = n;
	pow = 1;
	while (tmp /= 10)
		pow *= 10;
	while (pow)
	{
		c = n / pow + '0';
		io_write(w, &c, 1);
		n %= pow;
		pow /= 10;
	}
}

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

uint16_t	get_columns(int fd)
{
	struct winsize	w;

	if (ioctl(fd, TIOCGWINSZ, &w) == -1)
		return (UINT16_MAX);
	return (w.ws_col);
}

void		underline_error(t_write *err, t_span begin, t_span end, uintmax_t severity, char *line, size_t line_len)
{
	size_t i;

	i = 0;
	while (i < begin.col - 1 && i < line_len)
	{
		if (line[i] == '\t')
			io_write(err, "    ", 4);
		else
			io_write(err, " ", 1);
		i++;
	}
	if (severity == 1)
		io_write(err, CSI_RED, (sizeof(CSI_RED) - 1));
	else
		io_write(err, CSI_YELLOW, (sizeof(CSI_YELLOW) - 1));
	io_write(err, "^", 1);
	i++;
	while (i < end.col - 1 && i < line_len)
	{
		io_write(err, "^", 1);
		i++;
	}
}

#include <string.h>

void	error_contxt_print(t_write *err, t_span begin, t_span end, uintmax_t severity) //TODO add norm + werror management
{
	const uint16_t	columns = get_columns(2);
	size_t	fd;
	size_t	len;
	size_t	i;
	size_t	size;
	char	buffer[4096];

	i = 0;
	fd = open(begin.file_name, O_RDONLY);
	lseek(fd, begin.offset - begin.col + 1, SEEK_SET);
	len = end.col + 100;
	if (len > sizeof(buffer))
		len = sizeof(buffer);
	len = read(fd, buffer, len);
	size = 6;
	while (i < len  && buffer[i] != '\n' && size < columns - 1)
	{
		if (buffer[i] == '\t')
			size += 4;
		else
			size++;
		i++;
	}
	len = i;
	io_write(err, CSI_BLUE, sizeof(CSI_BLUE) - 1);
	io_write(err, "    ", 4 - nb_len(begin.lines % 1000));
	io_write_number(err, begin.lines % 1000);
	io_write(err, " " SEPARATOR " ", 2 + sizeof(SEPARATOR) - 1);
	io_write(err, CSI_RESET, (sizeof(CSI_RESET) - 1));
	char *last = buffer;
	char *tab;
	char *b_end = buffer + len;

	while ((tab = memchr(last, '\t', b_end - last)))
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
	underline_error(err, begin, end, severity, buffer, len);
}


int		print_small_error(t_read *in, uintmax_t severity, char *error)
{
	t_write	err;
	uint8_t	buffer[4096];

	err = (t_write){
		.buffer = buffer,
		.index = 0,
		.nbr_write = 0,
		.flushable = true,
		.fd = 2,
		.buffer_size = sizeof(buffer)
	};
	error_severity(&err, severity);
	error_msg(&err, error);
	io_write(&err, CSI_RESET, (sizeof(CSI_RESET) - 1));
	io_write(&err, "\n", 1);
	io_flush(&err);
	return (1);
}

void	print_error(t_read *in, uintmax_t severity, char *error, char *expected)
{
	uint8_t	buffer[4096];
	size_t  i;
	t_write err;

	err = (t_write){
		.buffer = buffer,
		.index = 0,
		.nbr_write = 0,
		.flushable = true,
		.fd = 2,
		.buffer_size = sizeof(buffer)
	};
	i = 0;

	if (severity == SEVERITY_ERROR || in->werror)
		in->write_able = false;
	error_severity(&err, severity);
	error_msg(&err, error);
	locate_error(&err, in->begin);
	error_contxt_print(&err, in->begin, in->span, severity);
	if (expected)
	{
		io_write(&err, " ", 1);
		io_write(&err, expected, ft_strlen(expected));
	}
	io_write(&err, CSI_RESET "\n\n", (sizeof(CSI_RESET) - 1 + 2));
	io_flush(&err);
}
