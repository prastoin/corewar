/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/04 14:14:11 by prastoin          #+#    #+#             */
/*   Updated: 2019/05/04 15:02:38 by prastoin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_string.h"
#include "asm.h"
#include <sys/ioctl.h>

#define MSG "Expected one of "
#define SEV_ERROR CSI_RED "error: "
#define SEV_WARNING CSI_YELLOW "warning: "

uint16_t	get_columns(int fd)
{
	struct winsize	w;

	if (ioctl(fd, TIOCGWINSZ, &w) == -1)
		return (UINT16_MAX);
	return (w.ws_col);
}

char		*from_int_to_type(size_t type)
{
	type &= PARAM_ALL;
	if (type == PARAM_ALL)
		return (MSG "Direct | Registre | Indirect");
	if (type == (PARAM_DIRECT | PARAM_INDIRECT))
		return (MSG "Direct | Indirect");
	if (type == (PARAM_DIRECT | PARAM_REGISTER))
		return (MSG "Direct | Registre");
	if (type == (PARAM_INDIRECT | PARAM_REGISTER))
		return (MSG "Indirect | Registre");
	if (type == PARAM_DIRECT)
		return ("Expected Direct");
	if (type == PARAM_REGISTER)
		return ("Expected Registre");
	if (type == PARAM_INDIRECT)
		return ("Expected Indirect");
	return (NULL);
}

void		error_severity(t_write *err, t_severity severity)
{
	if (severity == ERR)
		io_write(err, SEV_ERROR, sizeof(SEV_ERROR) - 1);
	if (severity == WARN)
		io_write(err, SEV_WARNING, sizeof(SEV_WARNING) - 1);
	io_write(err, CSI_RESET, sizeof(CSI_RESET) - 1);
}

void		error_msg(t_write *err, char *error)
{
	io_write(err, CSI_WHITE, sizeof(CSI_WHITE) - 1);
	io_write(err, error, ft_strlen(error));
}
