/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   play.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/04 10:13:41 by prastoin          #+#    #+#             */
/*   Updated: 2019/04/22 15:20:31 by prastoin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

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
//	t_span	begin; a implementer
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
	printf("%s a gagne avec le num %zu\n", champ[winner].name, winner);
	return (false);
}

bool	cycle_decremente_die(t_vm *vm)
{
	printf("Decremente NBR_LIVE = %zu\n", vm->nbr_live);
	if (vm->nbr_live >= NBR_LIVE || vm->check == MAX_CHECKS)
	{
		vm->cycle_to_die -= CYCLE_DELTA;
		vm->check = 0;
		vm->nbr_live = 0;
	}
	else
		vm->check++;
	if (vm->cycle_to_die <= 0)
	{
		printf("cycle to die = 0\n");
		return (ft_winner(vm->champ));
	}
	return (true);
}

bool	vm_cycle_to_die(t_vm *vm)
{
	size_t i;
	size_t dead;

	if (vm->i_to_die >= vm->cycle_to_die)
	{
		printf("DIE UPDATE | au cycle = %zu\n", vm->cycle);
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
		i = 0;
		while (i < vm->vec->len)
		{
			if (vm->vec->processes[i].said_live == true)
				vm->vec->processes[i].said_live = false;
			else
				vm->vec->processes[i].is_alive = false;
			if (vm->vec->processes[i].is_alive == false)
				dead++;
			if (dead == vm->vec->len)
				return (ft_winner(vm->champ));
			i++;
		}
		if (!cycle_decremente_die(vm))
			return (false);
	vm->i_to_die = 0;
	}
	vm->i_to_die++;
	vm->cycle++;
	return (true);
}

void	ft_dump_mem(t_vm vm, bool ex)
{
	size_t line;
	size_t i;

	line = 0;
	i = 0;
	printf("\n");
	while (i < MEM_SIZE)
	{
		if (i % 32 == 0)
		{
			if (i != 0)
				printf("\n");
			printf ("%.4lx\t", line);
			line += 32;
		}
		else if (i % 8 == 0 && i != 0)
			printf ("\t");
		if (vm.mem[i] == 0)
			printf("%.2x", vm.mem[i]);
		else
			printf("\033[32;01m%.2x\033[0m", vm.mem[i]);
		printf (" ");
		i++;
	}
	printf("\n");
	if (ex == true)
		exit (0);
}

void	david_needs_to_work(t_vm vm)
{
	ssize_t i;
	t_process *process;

	vm.continu = true;
	while (vm.continu)
	{
//		printf("FEU D'ARTIFICE\n");
		i = vm.vec->len - 1;
		printf("│champion[0] %s\n", vm.live[0] ? "is_alive" : "is_dead");
		printf("│champion[1] %s\n", vm.live[1] ? "is_alive" : "is_dead");
		while (i >= 0)
		{
			process = vm.vec->processes + i;
			printf("process[\033[32;01m%zu\033[0m] working at mem[\033[33;01m%4zu\033[0m]  \033[31m%.3zu\033[0m | \033[37;01m%.4zu / %.4zu\033[0m | \033[34;01m%d\033[0m \n", i, process->offset, process->cycle_to_do, vm.i_to_die, vm.cycle_to_die, process->is_alive ? 1 : 0);
			if (process->is_alive)
			{
				if (process->cycle_to_do == 0 && process->has_read)
				{
					printf("\033[37;01mFt_pass => Cycle_to_do = 0\n\033[0m");
					ft_pass(&vm, process);
					process = vm.vec->processes + i;
					printf("\n");
				}
				if (process->cycle_to_do > 0)
					process->cycle_to_do--;
			}
			i--;
		}
		i = 0;
		while (i < vm.vec->len)
		{
			process = vm.vec->processes + i;
			printf("%d: Cycle %d, has_read: %d\n", i, process->cycle_to_do, process->has_read);
			if (process->cycle_to_do == 0 && !process->has_read)
			{
				printf("\033[37;01mRead_opcode\n\033[0m");
				read_opcode(&vm, process);
				printf("\n");
			}
			i++;
		}
		if (vm.cycle == vm.flags.dump_c) {
			size_t i = 0;
			while (i < MEM_SIZE)
			{
				fprintf(stderr, "%02x", vm.mem[i]);
				i++;
				if (i != MEM_SIZE)
					fprintf(stderr, " ");
			}
			printf("\n");
			for (int i = 0; i < vm.vec->len; i++)
				printf("%d\n", vm.vec->processes[i].is_alive);
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
//	printf("%zu\n", vm.nbr_champ);
	while (nbr_champ < vm.nbr_champ)
	{
//		printf("vm.champ[i].fd = %zu\n", vm.champ[i].fd);
		if (vm.champ[i].fd)
		{
			if (!bin_parse_header(vm.champ[i].fd, vm.champ + i))
				return (false);
			nbr_champ++;
		}
		i++;
	}
	i = 0;
	nbr_champ = 0;
	vm.vec = create_process(MAX_PLAYERS);
	while (nbr_champ < vm.nbr_champ)
	{
		printf("%d < %d\n", nbr_champ, vm.nbr_champ);
		if (vm.champ[i].fd)
		{
			//chargement mem(MEM_SIZE / vm.nbr_champ) * nbr_champ)
			printf("avant = %d\n", vm.nbr_champ);
			ft_memcpy((vm.mem) + ((MEM_SIZE / vm.nbr_champ) * nbr_champ), vm.champ[i].prog, vm.champ[i].size);
			printf("apres = %d\n", vm.nbr_champ);
			//ajout process data
			process = add_process(&vm.vec);
			*process = (t_process) {
				.offset = (MEM_SIZE / vm.nbr_champ) * nbr_champ,
				.is_alive = true
			};
			conv_int_to_bin(~i, process->registre[1]);
			nbr_champ++;
		}
		i++;
	}
	i = 0;
	david_needs_to_work(vm);
	return (true);
}
