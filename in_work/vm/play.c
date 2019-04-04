/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   play.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/04 10:13:41 by prastoin          #+#    #+#             */
/*   Updated: 2019/04/04 16:29:28 by prastoin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

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

bool	bin_parse_header(size_t fd, t_champ *header)
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
	if (read(fd, header->prog, header->size) != header->size)
		return (false);
	return (true);
}

bool	ft_play(t_vm vm)
{
	size_t i;

	i = 0;
	while (i < vm.nbr_champ)
	{
		bin_parse_header(vm.champ[i].fd, &(vm.champ[i]));
		i++;
	}
	i = 0;
	vm.vec = create_process(MAX_PLAYERS);
	while (i < vm.nbr_champ)
	{
		ft_memcpy(vm.mem + ((MEM_SIZE / vm.nbr_champ) * i), vm.champ[i].prog, vm.champ[i].size);
		vm.vec->processes[i] = add_process(&(vm.vec), (MEM_SIZE / vm.nbr_champ) * i);
		i++;
	}
	i = 0;
	vm.continu = true;
	while (vm.continu)
	{
		i = 0;
		while (i < vm.vec->len)
		{
			
			i++;
		}
		vm.cycle++;
	}
	return (true);
}
