/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   play.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/04 10:13:41 by prastoin          #+#    #+#             */
/*   Updated: 2019/04/05 16:54:50 by prastoin         ###   ########.fr       */
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

bool	ft_winner(t_champ champ[MAX_PLAYERS])
{
	ssize_t		min;
	size_t		i;
	size_t		winner;

	i = 0;
	min = -1;
	while (i < MAX_PLAYERS)
	{
		if (min > champ[i].last_cycle_live || min == -1)
		{
			min = champ[i].last_cycle_live;
			winner = i;
		}
		i++;
	}
	printf("%s a gagne avec le num %zu\n", champ[i].name, i);
	return (false);
}

bool	cycle_decremente_die(t_vm *vm)
{
	if (vm->nbr_live == NBR_LIVE || vm->check == MAX_CHECKS)
	{
		vm->cycle_to_die -= CYCLE_DELTA;
		vm->check = 0;
		vm->nbr_live = 0;
	}
	else
		vm->check++;
	if (vm->cycle_to_die <= 0)
		return (ft_winner(vm->champ));
	return (true);
}

bool	vm_cycle_to_die(t_vm *vm)
{
	size_t i;
	size_t dead;

	if (vm->cycle == vm->cycle_to_die)
	{
		dead = 0;
		i = 0;
		while (i < MAX_PLAYERS)
		{
			if (vm->live[i] == true)
				vm->live[i] = false;
			else
				dead++;
			if (dead == MAX_PLAYERS)
				return (ft_winner(vm->champ));
		}
		i = 0;
		while (i < vm->vec->len)
		{
			if (vm->vec->processes[i].said_live == true)
				vm->vec->processes[i].said_live = false;
			else
				vm->vec->processes[i].is_alive = false;
		}
		if (!cycle_decremente_die(vm))
			return (false);
	}
	vm->cycle++;
	return (true);
}

void	david_needs_to_work(t_vm vm)
{
	size_t i;
	t_process process;

	vm.continu = true;
	while (vm.continu)
	{
		i = 0;
		while (i < vm.vec->len)
		{
			process = vm.vec->processes[i];
			if (process.is_alive)
			{
				if (process.cycle_to_do == 0 && process.has_read == false)
					read_opcode(&vm, vm.vec->processes + i);
				if (process.cycle_to_do == 0)
					ft_pass(&vm, vm.vec->processes + i);
				process.cycle_to_do--;
			}
			i++;
		}
		vm.continu = vm_cycle_to_die(&vm);
	}
	return ;
}

//TODO CLOSE TOUT LES FD

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
		conv_int_to_bin(i, vm.vec->processes[i].registre[1]);
		i++;
	}
	david_needs_to_work(vm);
	return (true);
}
