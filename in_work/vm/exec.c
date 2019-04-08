/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/02 14:20:14 by prastoin          #+#    #+#             */
/*   Updated: 2019/04/08 12:51:14 by prastoin         ###   ########.fr       */
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

t_process	add_process(t_vec **list, size_t offset)
{
	size_t	new_capacity;

	if ((*list)->len == (*list)->capacity)
	{
		new_capacity = (*list)->capacity * 2;
		*list = realloc(*list, sizeof(t_vec) + new_capacity * sizeof(t_process));
		(*list)->capacity = new_capacity;
	}
	(*list)->processes[(*list)->len++] = (t_process) {
		.offset = offset,
		.is_alive = true
	};
return ((*list)->processes[(*list)->len - 1]);
}

void		copy_process(t_process *dest, t_process *src)
{
	size_t i;

	i = 0;
	dest->carry = src->carry;
	dest->said_live = src->said_live;
	while (i < REG_NUMBER)
	{
		ft_memcpy(dest->registre + i, src->registre + i, REG_SIZE);
		i++;
	}
}
