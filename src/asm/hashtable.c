/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hashtable.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/20 13:58:43 by prastoin          #+#    #+#             */
/*   Updated: 2019/05/04 15:22:01 by prastoin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"
#include "ft_string.h"

static uint64_t	hash(char *str)
{
	uint64_t value;

	value = 456;
	while (*str)
	{
		value = value * 31 + *str;
		str++;
	}
	return (value);
}

t_entry			create_entry(char *key)
{
	return ((t_entry) {
		.key = key,
		.hash = hash(key)
	});
}

bool			grow_hashtable(t_hashtable **old)
{
	t_hashtable	*table;
	size_t		i;
	t_entry		*entry;

	if (!(table = create_hashtable((*old)->size * 2)))
		return (false);
	i = 0;
	while (i < (*old)->size)
	{
		entry = (*old)->bucket + i;
		if (entry->key)
			insert_hashtable(&table, *entry);
		i++;
	}
	free(*old);
	*old = table;
	return (true);
}

t_entry			*insert_hashtable(t_hashtable **table, t_entry entry) //TODO error management
{
	size_t			i;
	size_t			j;
	const t_entry	*curr = (*table)->bucket;

	j = 0;
	i = entry.hash % (*table)->size;
	while (curr[i % (*table)->size].key != NULL
			&& j < (*table)->size / 2)
	{
		if (entry.hash == curr[i % (*table)->size].hash)
			if (ft_strcmp(entry.key, curr[i % (*table)->size].key) == 0)
				return (NULL);
		i++;
		j++;
	}
	if (j == (*table)->size / 2)
	{
		if (!grow_hashtable(table))
			return (NULL);
		return (insert_hashtable(table, entry));
	}
	(*table)->bucket[i % (*table)->size] = entry;
	return ((*table)->bucket + (i % (*table)->size));
}

t_entry			*hashtable_get(t_hashtable *table, char *name)
{
	uint64_t		hash_name;
	size_t			i;
	size_t			j;
	t_entry *const	curr = table->bucket;

	hash_name = hash(name);
	i = hash_name % table->size;
	j = 0;
	while (j < table->size
			&& curr[i % table->size].key
			&& (curr[i % table->size].hash != hash_name
				|| ft_strcmp(curr[i % table->size].key, name) != 0))
	{
		i++;
		j++;
	}
	if (curr[i % table->size].key && j < table->size)
		return (curr + i % table->size);
	else
		return (NULL);
}
