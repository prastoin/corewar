/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/02 14:20:14 by prastoin          #+#    #+#             */
/*   Updated: 2019/04/26 11:49:13 by prastoin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

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
		*list = realloc(*list, sizeof(t_vec) + new_capacity * sizeof(t_process));
		(*list)->capacity = new_capacity;
	}
	return ((*list)->processes + (*list)->len++);
}

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
