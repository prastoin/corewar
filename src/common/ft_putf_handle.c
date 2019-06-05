/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putf_handle.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/06 00:15:15 by prastoin          #+#    #+#             */
/*   Updated: 2019/06/05 11:27:49 by prastoin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_string.h"

void			putnbr_hexa_fd(t_write *out, uintmax_t nb, size_t padd)
{
	const char	*base = "0123456789abcdef";
	uintmax_t	tmp;
	size_t		i;
	char		str[11];

	tmp = nb;
	i = 1;
	while (tmp /= 16)
		i++;
	str[i] = '\0';
	while (i--)
	{
		str[i] = base[nb % 16];
		nb /= 16;
	}
	str_hexa_fd(out, str, padd);
}

void			handle_hexa(t_write *out, va_list args, size_t padd, char *flag)
{
	uintmax_t	nb;

	if (*flag == 'x')
		nb = va_arg(args, uint32_t);
	else
		nb = va_arg(args, uint64_t);
	putnbr_hexa_fd(out, nb, padd);
}

void			handle_dlu(t_write *out, va_list args, size_t padd, char *flag)
{
	intmax_t	nb;

	if (*flag == 'd')
		nb = va_arg(args, int32_t);
	else if (*flag == 'u')
		nb = va_arg(args, uint32_t);
	else if (*flag == 'D')
		nb = va_arg(args, int64_t);
	else if (*flag == 'U')
		nb = va_arg(args, uint64_t);
	else
		return ;
	while (padd && padd-- > (nb < 0 ? nb_len(nb) + 1 : nb_len(nb)))
		char_fd(out, ' ', 0);
	putnbr_fd(out, nb);
}
