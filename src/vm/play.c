/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   play.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/04 10:13:41 by prastoin          #+#    #+#             */
/*   Updated: 2019/05/06 10:16:18 by dde-jesu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"
#include "ft_string.h"

intmax_t	conv_bin_num(uint8_t *mem, size_t len)
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
		return (false);
	return (true);
}

bool		ft_winner(t_champ champ[MAX_PLAYERS], t_vm *vm)
{
	ssize_t		max;
	size_t		i;
	size_t		winner;

	i = 0;
	max = -1;
	while (i < MAX_PLAYERS)
	{
		if (max <= (ssize_t)champ[i].last_cycle_live)
		{
			max = champ[i].last_cycle_live;
			winner = i;
		}
		i++;
	}
	if (vm->flags.verbose)
		ft_putf_fd(vm->v_fd, "Contestant %U, \"%s\", has won !\n", winner + 1,
				champ[winner].name);
	else
		ft_putf("%s a gagne avec le num %d\n", champ[winner].name, winner);
	return (false);
}

bool		cycle_decremente_die(t_vm *vm)
{
	if (vm->nbr_live >= NBR_LIVE || vm->check + 1 == MAX_CHECKS)
	{
		vm->cycle_to_die -= CYCLE_DELTA;
		if (vm->flags.verbose)
			ft_putf_fd(vm->v_fd, "Cycle to die is now %d\n", vm->cycle_to_die);
		vm->check = 0;
	}
	else
		vm->check++;
	if (vm->cycle_to_die <= 0)
	{
		return (ft_winner(vm->champ, vm));
	}
	vm->nbr_live = 0;
	return (true);
}

void		kill_player(t_vm *vm)
{
	size_t i;

	i = 0;
	while (i < MAX_PLAYERS)
	{
		if (vm->said_live[i] == true)
			vm->said_live[i] = false;
		else
			vm->live[i] = false;
		i++;
	}
}

bool		kill_process(t_vm *vm, size_t dead)
{
	ssize_t i;

	i = vm->vec->len - 1;
	while (i >= 0)
	{
		if (vm->vec->processes[i].said_live == true)
			vm->vec->processes[i].said_live = false;
		else if (vm->vec->processes[i].is_alive == true)
		{
			if (vm->flags.verbose)
				ft_putf_fd(vm->v_fd,
						"Process %d hasn't lived for %d cycles (CTD %d)\n",
						i + 1, vm->cycle
						- vm->vec->processes[i].last_cycle_live,
						vm->cycle_to_die);
			vm->vec->processes[i].is_alive = false;
		}
		if (vm->vec->processes[i].is_alive == false)
			dead++;
		if (dead == vm->vec->len)
			return (ft_winner(vm->champ, vm));
		i--;
	}
	return (true);
}

bool		vm_cycle_to_die(t_vm *vm)
{
	if (vm->i_to_die >= vm->cycle_to_die)
	{
		kill_player(vm);
		if (!kill_process(vm, 0))
			return (false);
		if (!cycle_decremente_die(vm))
			return (false);
		vm->i_to_die = 0;
	}
	vm->i_to_die++;
	vm->cycle++;
	if (vm->flags.verbose)
		ft_putf_fd(vm->v_fd, "It is now cycle %d\n", vm->cycle);
	return (true);
}

void		dump_mem(t_vm *vm)
{
	size_t i;
	size_t fd;

	if (!vm->flags.verbose)
		fd = 1;
	else
		fd = vm->v_fd;
	i = 0;
	while (i < MEM_SIZE)
	{
		if (i % 64 == 0 && i != 0)
			ft_putf_fd(fd, "\n");
		if (i % 64 == 0)
			ft_putf_fd(fd, "0x%4x : ", i);
		ft_putf_fd(fd, "%2x ", vm->mem[i]);
		i++;
	}
	ft_putf_fd(fd, "\n");
	exit(0);
}

void		exec_process(t_vm *vm)
{
	ssize_t		i;
	t_process	*process;

	i = vm->vec->len - 1;
	while (i >= 0)
	{
		process = vm->vec->processes + i;
		if (process->is_alive)
		{
			if (process->cycle_to_do == 0 && process->has_read)
			{
				vm->c_pc = i + 1;
				ft_pass(vm, process);
				process = vm->vec->processes + i;
			}
			if (process->cycle_to_do > 0)
				process->cycle_to_do--;
		}
		i--;
	}
}

void		david_needs_to_work(t_vm *vm)
{
	ssize_t		i;
	t_process	*process;

	vm->continu = true;
	while (vm->continu)
	{
		exec_process(vm);
		i = 0;
		while ((size_t)i < vm->vec->len)
		{
			vm->c_pc = i + 1;
			process = vm->vec->processes + i;
			if (process->cycle_to_do == 0 && !process->has_read)
				read_opcode(vm, process);
			i++;
		}
		if (vm->cycle == (uintmax_t)vm->flags.dump_c)
			dump_mem(vm);
		vm->continu = vm_cycle_to_die(vm);
	}
	return ;
}

/*
**	TODO CLOSE TOUT LES FD
*/

void		affstart_verbose(t_vm vm)
{
	size_t		i;
	t_champ		champ;

	i = 0;
	ft_putf_fd(vm.v_fd, "Introducing contestants...\n");
	while (i < MAX_PLAYERS)
	{
		if (vm.live[i])
		{
			champ = vm.champ[i];
			ft_putf_fd(vm.v_fd,
					"* Player %U, weighing %U bytes, \"%s\" (\"%s\") !\n",
					i + 1, champ.size, champ.name, champ.comment);
		}
		i++;
	}
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
					.is_alive = true
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
				return (false);
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
