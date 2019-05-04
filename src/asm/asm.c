/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asm_read.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/14 11:34:39 by prastoin          #+#    #+#             */
/*   Updated: 2019/05/04 13:36:08 by prastoin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_string.h"
#include "asm.h"
#include <fcntl.h>
#include <limits.h>

#define FIX_SIZE_ERR "Program too big (Exceed CHAMP_MAX_SIZE)"

char		*change_ext(char *name)
{
	static char	file[PATH_MAX - 1];
	char		*dot;

	if (!(dot = ft_strrchr(name, '.')))
		return (NULL);
	if (!*(dot + 1) || *(dot + 1) != 's')
		return (NULL);
	if ((dot - name + (sizeof(EXT) - 1)) > PATH_MAX)
		return (NULL);
	ft_memcpy(file, name, dot - name);
	ft_memcpy(file + (dot - name), EXT, sizeof(EXT));
	return (file);
}

void		read_fixed(t_read *in, char *name)
{
	uint8_t		buffer[CHAMP_MAX_SIZE + HEADER_SIZE];
	t_write		out;

	out = init_write();
	out.buffer_size = CHAMP_MAX_SIZE + HEADER_SIZE;
	out.buffer = buffer;
	out.fd = 0;
	asm_transform(&out, in);
	if (out.fd == -1)
		print_small_error(in, ERR, FIX_SIZE_ERR, 0);
	else if (in->write_able)
	{
		if ((out.fd = open(name, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR))
				!= -1)
		{
			write(out.fd, out.buffer, out.nbr_write);
			close(out.fd);
			ft_putf("done static\n");
		}
		else
			print_small_error(in, ERR, "Cannot open output file", name);
	}
}

void		read_streaming(t_read *in, char *name)
{
	uint8_t		buffer[BUFFER_SIZE];
	t_write		out;

	out = init_write();
	out.buffer_size = BUFFER_SIZE;
	out.flushable = true;
	out.buffer = buffer;
	if ((out.fd = open(name, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR))
			!= -1)
	{
		asm_transform(&out, in);
		close(out.fd);
		ft_putf("done streaming\n");
	}
	else
		print_small_error(in, ERR, "Cannot open output file", name);
}

int			main(int argc, char *argv[])
{
	t_flag		flag;
	char		*files[2];
	int			fd;
	t_read		in;
	const t_arg	args[] = {
		{ARG_BOOLEAN, 's', "streaming", &flag.streaming, FLAG_S_MSG},
		{ARG_BOOLEAN, 'e', "Werror", &in.werror, "Warnings become errors"},
		{ARG_END, 0, 0, 0, 0}};

	flag = (t_flag) {0, 0};
	in.werror = false;
	if ((fd = parse_args(args, argc, argv)) < 0 || argc == fd)
		return (args_usage(args, argv[0], "source_file", ARGS_MSG));
	files[IN] = argv[fd];
	if (!(files[OUT] = change_ext(files[IN])))
		return (print_small_error(&in, ERR, "Invalid file name", files[IN]));
	if ((fd = open(files[IN], O_RDONLY)) <= 0)
		return (print_small_error(&in, ERR, "Open failed", files[IN]));
	in = init_read(fd, files[IN], in.werror);
	(flag.streaming ? read_streaming : read_fixed)(&in, files[OUT]);
	close(fd);
	return (0);
}
