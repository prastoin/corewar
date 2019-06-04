/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   close_fd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbecerri <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/06 01:33:38 by fbecerri          #+#    #+#             */
/*   Updated: 2019/06/05 10:00:15 by dde-jesu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

void		close_fd(t_vm *vm)
{
	size_t i;
	size_t fd;

	i = 0;
	while (i < MAX_PLAYERS)
	{
		fd = vm->champ[i].fd;
		if (fd)
			close(fd);
		i++;
	}
	if (vm->flags.verbose)
		close(vm->v.fd);
}
