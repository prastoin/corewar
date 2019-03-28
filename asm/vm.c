/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vm.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/28 09:01:43 by prastoin          #+#    #+#             */
/*   Updated: 2019/03/28 15:52:26 by prastoin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"
#include "vm.h"
#include "ft_string.h"

uint32_t		conv_bin_num(uint8_t *str, uint8_t len)
{
	uint8_t		i;
	uint32_t	nb;

	nb = 0;
	while (i < len)
	{
		nb += nb * 0x100 + str[i];
		i++;
	}
	return (nb);
}

bool	bin_parse_header(size_t fd, t_header *header)
{
	t_span	begin;
	uint8_t	magic_len[4];

	if (read(fd, magic_len, 4) != 4)
		return (false);
	if (conv_bin_num(magic_len, 4) != COREWAR_EXEC_MAGIC)
		return (false);
	if (read(fd, header->name, PROG_NAME_LENGTH) != PROG_NAME_LENGTH)
		return (false);
	lseek(fd, 4 - sizeof(header->name) % 4, SEEK_CUR);
	if (read(fd, magic_len, 4) != 4)
		return (false);
	header->size = conv_bin_num(magic_len, 4);
	if (read(fd, header->comment, COMMENT_LENGTH) != COMMENT_LENGTH)
		return (false);
	lseek(fd, 4 - sizeof(header->comment) % 4, SEEK_CUR);
	return (true);
}

bool	parse_long(const t_arg args[], char *arg, char *argv[], size_t *i)
{
	return (true);
}

bool	parse_short(const t_arg args[], char *arg, char *argv[], size_t *i)
{
	return (true);
}

bool	parse_args(const t_arg args[], int argc, char *argv[])
{
	size_t	i;
	char	*arg;

	i = 0;
	while (i < argc)
	{
		arg = argv[i];
		if (arg[0] != '-' || !arg[1])
			return (true);
		if (arg[1] != '-')
		{
			while (*++arg)
				if (!parse_short(args, arg, argv, &i))
					return (false);
		}
		else
		{
			if (!arg[2])
				return (false);
			arg += 2;
			if (!parse_long(args, arg, argv, &i))
				return (false);
		}
	}
	return (true);
}

int main(int argc, char *argv[])
{
	uint8_t		mem[MEM_SIZE];
	t_champ		champ[MAX_PLAYERS];
	size_t		i;
	size_t		nbr_player;
	t_flags		flags;

	const t_arg args[] = {
	{ARG_STRUCT, 'n', "number", &flags.num, "Choose the number for a player"},
	{ARG_INT, 'd', "dump", &flags.dump_c, "On sait pas on parse"},
	{ARG_INT, 'r', "run", &flags.run_c, "On sait pas on parse"},
	{ARG_BOOLEAN, 'b', "bin_aff", &flags.bin_o, "Affichage binnaire"},
	{ARG_BOOLEAN, 'c', "ncurse_aff", &flags.ncurse_o, "Affichage Ncurse"},
	{ARG_END, 0, 0, 0, 0}
	};

	i = 0;
	parse_args(args, argc, argv);
	while (i < nbr_player)
	{
		champ[i].fd = open((const char *)champ[i].name, O_RDONLY);
		bin_parse_header(champ[i].fd, &(champ[i].header));
		if (read(champ[i].fd, mem + (2), champ[i].header.size) != champ[i].header.size)
			return (0);
		i++;
	}
	return (0);
}
