/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <dde-jesu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/02 14:20:14 by prastoin          #+#    #+#             */
/*   Updated: 2019/05/22 22:49:09 by prastoin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"
#include "ft_string.h"
#ifndef __wasm__
# include <stdlib.h>
#endif

void		copy_process(t_process *dest, t_process *src)
{
	size_t i;

	i = 0;
	dest->carry = src->carry;
	dest->said_live = src->said_live;
	dest->last_cycle_live = src->last_cycle_live;
	while (i < REG_NUMBER)
	{
		ft_memcpy(dest->registre + i, src->registre + i, REG_SIZE);
		i++;
	}
}

#ifndef __wasm__

t_vec		*create_process(size_t capacity)
{
	t_vec	*list;

	list = malloc(sizeof(t_vec) + capacity * sizeof(t_process));
	*list = (t_vec) {
		.capacity = capacity,
		.len = 0
	};
	return (list);
}

t_process	*add_process(t_vec **list)
{
	size_t	new_capacity;

	if ((*list)->len == (*list)->capacity)
	{
		new_capacity = (*list)->capacity * 2;
		*list = realloc(*list, sizeof(t_vec)
				+ new_capacity * sizeof(t_process));
		(*list)->capacity = new_capacity;
	}
	return ((*list)->processes + (*list)->len++);
}

#else
# define PAGESIZE (1024*64)

extern char __heap_base;

t_vec		*create_process(size_t capacity)
{
	t_vec	*list;

	list = (t_vec *)&__heap_base;
	*list = (t_vec) {
		.capacity = (__builtin_wasm_memory_size(0) * PAGESIZE - (uintptr_t)&__heap_base - sizeof(t_vec)) / sizeof(t_process),
		.len = 0
	};
	return (list);
}

t_process	*add_process(t_vec **list)
{
	if ((*list)->len == (*list)->capacity)
	{
		if (((*list)->capacity * sizeof(t_vec)) / PAGESIZE)
			__builtin_wasm_memory_grow(0, ((*list)->capacity * sizeof(t_vec)) / PAGESIZE);
		else
			__builtin_wasm_memory_grow(0, 1);
		(*list)->capacity = (__builtin_wasm_memory_size(0) * PAGESIZE - (uintptr_t)&__heap_base - sizeof(t_vec)) / sizeof(t_process);
	}
	return ((*list)->processes + (*list)->len++);
}

#endif
