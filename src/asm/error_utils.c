/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/04 14:14:11 by prastoin          #+#    #+#             */
/*   Updated: 2019/05/22 22:37:25 by prastoin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_string.h"
#include "asm.h"
#include <sys/ioctl.h>

uint16_t	get_columns(int fd)
{
	struct winsize	w;

	if (ioctl(fd, TIOCGWINSZ, &w) == -1)
		return (UINT16_MAX);
	return (w.ws_col);
}

char		*from_int_to_type(size_t type)
{
	type &= Param_All;
	if (type == Param_All)
		return (MSG "Direct | Registre | Indirect");
	if (type == (Param_Direct | Param_Indirect))
		return (MSG "Direct | Indirect");
	if (type == (Param_Direct | Param_Register))
		return (MSG "Direct | Registre");
	if (type == (Param_Indirect | Param_Register))
		return (MSG "Indirect | Registre");
	if (type == Param_Direct)
		return ("Expected Direct");
	if (type == Param_Register)
		return ("Expected Registre");
	if (type == Param_Indirect)
		return ("Expected Indirect");
	return (NULL);
}

void		error_severity(t_write *error, t_severity severity)
{
	if (severity == Err)
		io_write(error, SEV_ERROR, sizeof(SEV_ERROR) - 1);
	if (severity == Warn)
		io_write(error, SEV_WARNING, sizeof(SEV_WARNING) - 1);
	io_write(error, CSI_RESET, sizeof(CSI_RESET) - 1);
}

void		error_msg(t_write *error, char *error_str)
{
	io_write(error, CSI_WHITE, sizeof(CSI_WHITE) - 1);
	io_write(error, error_str, ft_strlen(error_str));
}
