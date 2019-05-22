/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pre_start.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbecerri <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/06 01:45:36 by fbecerri          #+#    #+#             */
/*   Updated: 2019/05/22 23:15:34 by prastoin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"
#include "ft_string.h"

bool		bin_parse_header(size_t fd, t_champ *header)
{
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
	if ((header->size = conv_bin_num(magic_len, 4)) > CHAMP_MAX_SIZE)
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

void		init_processes(t_vm *vm)
{
	size_t		nbr_champ;
	t_process	*process;
	size_t		i;

	i = 0;
	nbr_champ = 0;
	vm->vec = create_process(MAX_PLAYERS);
	while (nbr_champ < vm->nbr_champ)
	{
		if (vm->champ[i].fd)
		{
			ft_memcpy((vm->mem) + ((MEM_SIZE / vm->nbr_champ) * nbr_champ),
					vm->champ[i].prog, vm->champ[i].size);
			process = add_process(&vm->vec);
			*process = (t_process) {
				.offset = (MEM_SIZE / vm->nbr_champ) * nbr_champ,
				.is_alive = true,
				.cycle_to_do = 1
			};
			conv_int_to_bin(~i, process->registre[0]);
			nbr_champ++;
		}
		i++;
	}
}

bool		ft_play(t_vm vm)
{
	size_t	i;
	size_t	nbr_champ;

	i = 0;
	nbr_champ = 0;
	while (nbr_champ < vm.nbr_champ)
	{
		if (vm.champ[i].fd)
		{
			if (!bin_parse_header(vm.champ[i].fd, vm.champ + i))
			{
				ft_putf_fd(2, "Error in %s's header\n", vm.champ[i].name);
				return (false);
			}
			nbr_champ++;
		}
		i++;
	}
	init_processes(&vm);
	i = 0;
	if (vm.flags.verbose)
		affstart_verbose(vm);
	david_needs_to_work(&vm);
	return (true);
}
