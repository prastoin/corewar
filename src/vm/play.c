/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   play.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/04 10:13:41 by prastoin          #+#    #+#             */
/*   Updated: 2019/04/29 12:07:14 by prastoin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"
#include "ft_string.h"

intmax_t	conv_bin_num(uint8_t *mem, size_t len) //TODO gestion bit de signe pour faire des lives de negatif
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
	if (mem[0] & 0x80) {
		while (i < sizeof(num))
		{
			num |= ((uintmax_t)0xFF << (i * 8));
			i++;
		}
	}
	return (num);
}

bool	bin_parse_header(size_t fd, t_champ *header)
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
	if ((header->size = conv_bin_num(magic_len, 4)) > CHAMP_MAX_SIZE) //verifier si taille decrite dans le header est la meme que celle du fichier TODO DAVID NEEDS TO WORK
		return (false);
	if (read(fd, header->comment, COMMENT_LENGTH) != COMMENT_LENGTH)
		return (false);
	lseek(fd, 4 - sizeof(header->comment) % 4, SEEK_CUR);
	if (read(fd, header->prog, header->size) != (ssize_t)header->size)
		return (false);
	return (true);
}

bool	ft_winner(t_champ champ[MAX_PLAYERS])
{
	ssize_t		max;
	size_t		i;
	size_t		winner;

	i = 0;
	max = -1;
	while (i < MAX_PLAYERS)
	{
		if (max < (ssize_t)champ[i].last_cycle_live)
		{
			max = champ[i].last_cycle_live;
			winner = i;
		}
		i++;
	}
	ft_putf("%s a gagne avec le num %d\n", champ[winner].name, winner);
	return (false);
}

bool	cycle_decremente_die(t_vm *vm)
{
	if (vm->nbr_live >= NBR_LIVE || vm->check + 1  == MAX_CHECKS)
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
		return (ft_winner(vm->champ));
	}
	vm->nbr_live = 0;
	return (true);
}

bool	vm_cycle_to_die(t_vm *vm)
{
	ssize_t i;
	size_t dead;

	if (vm->i_to_die >= vm->cycle_to_die)
	{
		dead = 0;
		i = 0;
		while (i < MAX_PLAYERS)
		{
			if (vm->said_live[i] == true)
				vm->said_live[i] = false;
			else
				vm->live[i] = false;
		i++;
		}
		i = vm->vec->len - 1;
		while (i >= 0)
		{
			if (vm->vec->processes[i].said_live == true)
				vm->vec->processes[i].said_live = false;
			else if (vm->vec->processes[i].is_alive == true)
			{
				if (vm->flags.verbose)
					ft_putf_fd(vm->v_fd, "Process %d hasn't lived for %d cycles (CTD %d)\n", i + 1, vm->cycle - vm->vec->processes[i].last_cycle_live, vm->cycle_to_die);
				vm->vec->processes[i].is_alive = false;
			}
			if (vm->vec->processes[i].is_alive == false)
				dead++;
			if (dead == vm->vec->len)
				return (ft_winner(vm->champ));
			i--;
		}
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

void	david_needs_to_work(t_vm vm)
{
	ssize_t i;
	t_process *process;

	g_vm = &vm;
	vm.continu = true;
	while (vm.continu)
	{
		i = vm.vec->len - 1;
		while (i >= 0)
		{
			process = vm.vec->processes + i;
			if (process->is_alive)
			{
				if (process->cycle_to_do == 0 && process->has_read)
				{
					vm.c_pc = i + 1;
					ft_pass(&vm, process);
					process = vm.vec->processes + i;
				}
				if (process->cycle_to_do > 0)
					process->cycle_to_do--;
			}
			i--;
		}
		i = 0;
		while ((size_t)i < vm.vec->len)
		{
			process = vm.vec->processes + i;
			if (process->cycle_to_do == 0 && !process->has_read)
				read_opcode(&vm, process);
			i++;
		}
		if (vm.cycle == vm.flags.dump_c)
		{
			i = 0;
			while (i < MEM_SIZE)
			{
				fprintf(stderr, "%02x", vm.mem[i]);
				i++;
				if (i != MEM_SIZE)
					fprintf(stderr, " ");
			}
			exit(0);
		}
		vm.continu = vm_cycle_to_die(&vm);
	}
	return ;
}

//TODO CLOSE TOUT LES FD

bool	ft_play(t_vm vm)
{
	size_t	i;
	size_t	nbr_champ;
	t_process	*process;

	i = 0;
	nbr_champ = 0;
	while (nbr_champ < vm.nbr_champ)
	{
		if (vm.champ[i].fd)
		{
			if (!bin_parse_header(vm.champ[i].fd, vm.champ + i)) //deleting ungood champ
				return (false);
		}
		i++;
	}
	i = 0;
	nbr_champ = 0;
	vm.vec = create_process(MAX_PLAYERS);
	while (nbr_champ < vm.nbr_champ)
	{
		if (vm.champ[i].fd)
		{
			//chargement mem(MEM_SIZE / vm.nbr_champ) * nbr_champ)
			ft_memcpy((vm.mem) + ((MEM_SIZE / vm.nbr_champ) * nbr_champ), vm.champ[i].prog, vm.champ[i].size);
			//ajout process data
			process = add_process(&vm.vec);
			*process = (t_process) {
				.offset = (MEM_SIZE / vm.nbr_champ) * nbr_champ,
				.is_alive = true
			};
			conv_int_to_bin(~i, process->registre[0]);
			nbr_champ++;
		}
		i++;
	}
	i = 0;
	david_needs_to_work(vm);
	return (true);
}
