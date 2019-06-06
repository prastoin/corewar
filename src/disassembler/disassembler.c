/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbecerri <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/13 11:13:26 by fbecerri          #+#    #+#             */
/*   Updated: 2019/06/06 12:31:52 by prastoin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include "disassembler.h"

intmax_t		conv_bin(uint8_t *mem, size_t len)
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

bool			bin_parse_header(size_t fd, t_head *header)
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
		return (error_message("Declared prog size is false"));
	return (true);
}

bool			begin_diss(t_diss diss)
{
	t_head			header;
	t_read_prog		prog;

	prog.offset = 0;
	if (!bin_parse_header(diss.fd_in, &header))
		return (error_message("Parse header"));
	ft_putf_fd(diss.fd_out, ".name      \"%s\"\n", header.name);
	ft_putf_fd(diss.fd_out, ".comment   \"%s\"\n\n", header.comment);
	while (prog.offset < header.size)
	{
		if (!(diss_read_opcode(header.prog, &prog)))
			return (error_message("Invalid opcode"));
		if (!diss_pass(header.prog, &prog, diss))
			return (error_message("Invalid ocp"));
	}
	return (true);
}

static t_arg	*create_args(char **name)
{
	static t_arg args[2];

	args[0] = (t_arg){
		Arg_Str, 'o', "output", &(*name), "Output file name."
	};
	args[1] = (t_arg){
		Arg_End, 0, 0, 0, 0
	};
	return (args);
}

int				main(int argc, char *argv[])
{
	t_diss		diss;
	char		*name;
	int			ret;
	t_arg		*args;

	name = NULL;
	args = create_args(&name);
	if ((ret = parse_args(args, argc, argv)) < 0 || argc != ret + 1)
		return (args_usage(args, argv[0], "cor_file", ""));
	if ((diss.fd_in = open(argv[ret], O_RDONLY)) <= 0)
		return (error_message("Open failed"));
	diss.fd_out = 1;
	if (name && (diss.fd_out = open(name, O_RDWR
					| O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR)) <= 0)
		return (error_message("Created failed"));
	begin_diss(diss);
	return (0);
}
