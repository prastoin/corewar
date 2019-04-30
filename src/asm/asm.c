/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asm_read.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/14 11:34:39 by prastoin          #+#    #+#             */
/*   Updated: 2019/04/30 11:30:39 by prastoin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_string.h"
#include "asm.h"
#include <fcntl.h>
#include <limits.h>

char			*change_ext(char *name)
{
	static char	file[PATH_MAX - 1];
	char		*dot;

	if (!(dot = ft_strrchr(name, '.')))
		dot = name + ft_strlen(name);
	if ((dot - name + (sizeof(EXT) - 1)) > PATH_MAX)
		return (NULL);
	ft_memcpy(file, name, dot - name);
	ft_memcpy(file + (dot - name), EXT, sizeof(EXT));
	return (file);
}

bool	ft_header(t_write *out, t_read *in)
{
	t_header		head;

	head = (t_header) {
		.size = 0
	};
	if (!asm_parse_header(in, &head))
		return (false);
	write_header(&head, out);
	return (true);
}

void		read_fixed(t_read *in, char *name)
{
	uint8_t	buffer[CHAMP_MAX_SIZE + HEADER_SIZE];
	t_write	out;
	t_hashtable		*table;

	out = init_write();
	out.buffer_size = CHAMP_MAX_SIZE + HEADER_SIZE;
	out.buffer = buffer;
	table = create_hashtable(8);
	if (!asm_parser(&out, in, table))
		return ;
	if (in->write_able == true)
	{
		out.fd = open(name, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
		write (out.fd, out.buffer, out.nbr_write);
		close(out.fd);
		ft_putf("done static\n");
	}
	return ;
}

void		read_streaming(t_read *in, char *name)
{
	uint8_t		buffer[BUFFER_SIZE];
	t_write		out;
	t_hashtable		*table;

	out = init_write();
	out.buffer_size = BUFFER_SIZE;
	out.flushable = true;
	out.buffer = buffer;
	out.fd = open(name, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
	table = create_hashtable(8);
	asm_parser(&out, in, table);
	close(out.fd);
	printf("done streaming\n");
}

int main(int argc, char *argv[])
{
	t_flag		flag;
	char		*file;
	char		*out;
	int			fd;
	ssize_t		ret;
	t_read		in;
	const t_arg	args[] = {
		{ARG_BOOLEAN, 's', "streaming", &flag.streaming, "Streaming on mode with this flag"},
		{ARG_BOOLEAN, 'e', "Werror", &in.werror, "Warnings become errors"},
		{ARG_END, 0, 0, 0, 0}};

	in.werror = false;
	flag = (t_flag) {0};
	if ((ret = parse_args(args, argc, argv)) < 0 || argc == ret)
		return (args_usage(args, argv[0], "source_file", "Convert asm to corewar bytecode"));
	file = argv[ret];
	if (!(out = change_ext(file)))
		return (print_small_error(&in, ERR, "Invalid name file"));
	if ((fd = open(file, O_RDONLY)) <= 0)
		return (print_small_error(&in, ERR, "Open failed"));
	in = init_read(fd, file, in.werror);
	(flag.streaming ? read_streaming : read_fixed)(&in, out);
	close(fd);
	return (0);
}
