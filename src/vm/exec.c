/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/02 14:20:14 by prastoin          #+#    #+#             */
/*   Updated: 2019/05/04 14:17:13 by dde-jesu         ###   ########.fr       */
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
# define P_CAPACITY (30000)

static uint8_t	g_memory[sizeof(t_vec) + P_CAPACITY * sizeof(t_process)];

t_vec		*create_process(size_t capacity)
{
	t_vec	*list;

	(void)capacity;
	list = (t_vec *)g_memory;
	*list = (t_vec) {
		.capacity = P_CAPACITY,
		.len = 0
	};
	return (list);
}

t_process	*add_process(t_vec **list)
{
	if ((*list)->len == (*list)->capacity)
		__builtin_trap();
	return ((*list)->processes + (*list)->len++);
}

#endif
