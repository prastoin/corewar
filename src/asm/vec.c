#include "asm.h"
#include <stdlib.h>

t_pos_vec	*create_pos_vec(size_t capacity)
{
	t_pos_vec	*vec;

	if (!(vec = malloc(sizeof(t_pos_vec) + capacity * sizeof(t_pos))))
		return (NULL);
	*vec = (t_pos_vec) {
		.capacity = capacity,
		.len = 0
	};
	return (vec);
}

t_pos	*add_position(t_pos_vec **list)
{
	size_t			new_capacity;
	t_pos_vec*	tmp;

	tmp = *list;
	if (tmp->len == tmp->capacity)
	{
		new_capacity = tmp->capacity * 2;
		if (!(tmp = realloc(tmp, sizeof(t_pos_vec)
						+ new_capacity * sizeof(t_pos))))
			return (NULL);
		tmp->capacity = new_capacity;
		*list = tmp;
	}
	return (tmp->inner + tmp->len++);
}
