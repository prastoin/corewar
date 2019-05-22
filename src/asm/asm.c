/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asm.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <dde-jesu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/14 11:34:39 by prastoin          #+#    #+#             */
/*   Updated: 2019/05/22 12:17:46 by dde-jesu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_string.h"
#include "asm.h"
#include <fcntl.h>
#include <limits.h>

#define FIX_SIZE_ERR "Program too big (Exceed CHAMP_MAX_SIZE)"

char	*change_ext(char *name)
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

int		read_fixed(t_read *in, char *name)
{
	uint8_t		buffer[CHAMP_MAX_SIZE + HEADER_SIZE];
	t_write		out;

	out = init_write();
	out.buffer_size = CHAMP_MAX_SIZE + HEADER_SIZE;
	out.buffer = buffer;
	out.fd = 0;
	if (!asm_transform(&out, in) && out.fd == -1)
		return (print_small_error(in, ERR, FIX_SIZE_ERR, 0));
	else if (in->write_able)
	{
		if ((out.fd = open(name, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR))
				!= -1)
		{
			write(out.fd, out.buffer, out.nbr_write);
			close(out.fd);
			ft_putf("done static\n");
			return (0);
		}
		else
			return (print_small_error(in, ERR, "Cannot open output", name));
	}
	else
		return (1);
}

int		read_streaming(t_read *in, char *name)
{
	uint8_t		buffer[BUFFER_SIZE];
	t_write		out;
	bool		ret;

	out = init_write();
	out.buffer_size = BUFFER_SIZE;
	out.flushable = true;
	out.buffer = buffer;
	if ((out.fd = open(name, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR))
			!= -1)
	{
		if ((ret = asm_transform(&out, in)))
			ft_putf("done streaming\n");
		close(out.fd);
		return (ret ? 0 : 1);
	}
	else
		return (print_small_error(in, ERR, "Cannot open output file", name));
}

int		main(int argc, char *argv[])
{
	t_flag		flag;
	char		*files[2];
	int			i;
	t_read		in;
	const t_arg	args[] = {
		{ARG_BOOLEAN, 's', "streaming", &flag.streaming, FLAG_S_MSG},
		{ARG_BOOLEAN, 'e', "Werror", &in.werror, "Warnings become errors"},
		{ARG_END, 0, 0, 0, 0}};

	flag = (t_flag) {0, 0};
	in.werror = false;
	if ((i = parse_args(args, argc, argv)) < 0 || argc != i + 1)
		return (args_usage(args, argv[0], "source_file", ARGS_MSG));
	files[IN] = argv[i];
	if (!(files[OUT] = change_ext(files[IN])))
		return (print_small_error(&in, ERR, "Invalid file name", files[IN]));
	if ((i = open(files[IN], O_RDONLY)) <= 0)
		return (print_small_error(&in, ERR, "Open failed", files[IN]));
	in = init_read(i, files[IN], in.werror);
	i = (flag.streaming ? read_streaming : read_fixed)(&in, files[OUT]);
	close(i);
	return (i);
}
