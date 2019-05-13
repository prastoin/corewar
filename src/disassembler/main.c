/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbecerri <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/13 11:13:26 by fbecerri          #+#    #+#             */
/*   Updated: 2019/05/13 14:49:40 by fbecerri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_string.h"
#include "asm.h"
#include "stdlib.h"
#include "fcntl.h"
#include "limits.h"
#include "disassembler.h"

intmax_t	conv_bin(uint8_t *mem, size_t len)
{
	uintmax_t	num;
	size_t		i;

	i = 0;
	num = 0;
	while (i < len)
	{
		num <<= 8;
		num |= mem[i];
		i++;
	}
	if (mem[0] & 0x80)
	{
		while (i < sizeof(num))
		{
			num |= ((uintmax_t)0xFF << (i * 8));
			i++;
		}
	}
	return (num);
}

bool		error_message(const char *err)
{
	ft_putf_fd(2, "%s%s%s\n", CSI_RED, err, CSI_RESET);
	return (false);
}

bool		bin_parse_header(size_t fd, t_head *header)
{
	uint8_t	magic_len[4];

	if (read(fd, magic_len, 4) != 4)
		return (false);
	if (conv_bin(magic_len, 4) != COREWAR_EXEC_MAGIC)
		return (false);
	if (read(fd, header->name, PROG_NAME_LENGTH) != PROG_NAME_LENGTH)
		return (false);
	lseek(fd, 4 - sizeof(header->name) % 4, SEEK_CUR);
	if (read(fd, magic_len, 4) != 4)
		return (false);
	if ((header->size = conv_bin(magic_len, 4)) > CHAMP_MAX_SIZE)
		return (false);
	if (read(fd, header->comment, COMMENT_LENGTH) != COMMENT_LENGTH)
		return (false);
	lseek(fd, 4 - sizeof(header->comment) % 4, SEEK_CUR);
	if (read(fd, header->prog, header->size + 1) != (ssize_t)header->size)
	{
		ft_putf_fd(2, "Declared prog size is false.\n");
		return (false);
	}
	return (true);
}

char		*change_ext(char *name)
{
	static char	file[PATH_MAX - 1];
	char		*dot;

	if (!(dot = ft_strrchr(name, '.')))
	{
		printf("no .\n");
		return (NULL);
	}
	if ((dot - name + (sizeof(DEXT) - 1)) > PATH_MAX)
	{
		printf("no .\n");
		return (NULL);
	}
	ft_memcpy(file, name, dot - name);
	ft_memcpy(file + (dot - name), DEXT, sizeof(DEXT));
	return (file);
}

bool	begin_diss(t_diss diss)
{
	t_head			header;
	t_read_prog		prog;

	prog.offset = 0;
	if (!bin_parse_header(diss.fd_in, &header))
		printf("error\n");
	ft_putf_fd(diss.fd_out, ".name      \"%s\"\n", header.name);
	ft_putf_fd(diss.fd_out, ".comment   \"%s\"\n\n", header.comment);
	while (prog.offset < header.size)
	{
		if (!(diss_read_opcode(header.prog, &prog)))
			return (false);
		if (!diss_pass(header.prog, &prog, diss))
			return (false);
	}
	return (true);
}

int		main(int argc, char *argv[])
{
	t_diss	diss;
	char	*name;
	int		ret;
	const t_arg args[] = {
		{ARG_STR, 'o', "output", &name, "Output file name"}
	};

	name = NULL;
	if ((ret = parse_args(args, argc, argv)) < 0 || argc != ret + 1)
		return (args_usage(args, argv[0], "cor_file", ""));
	if ((diss.fd_in = open(argv[ret], O_RDONLY)) <= 0)
		return (error_message("Open failed"));
	diss.fd_out = 1;
	if (name && (diss.fd_out = open(name, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR)) <= 0)
		return (error_message("Created failed"));
	begin_diss(diss);
	return (0);
}
