/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hashtable_create.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/04 13:37:30 by prastoin          #+#    #+#             */
/*   Updated: 2019/05/22 21:49:48 by prastoin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"
#include <stdlib.h>

t_hashtable		*create_hashtable(size_t size)
{
	t_hashtable *hash;
	size_t		i;

	if (!(hash = malloc(sizeof(*hash) + size * sizeof(*hash->bucket))))
		return (NULL);
	i = 0;
	while (i < size)
	{
		hash->bucket[i].key = NULL;
		i++;
	}
	hash->size = size;
	return (hash);
}
