/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/13 17:54:07 by prastoin          #+#    #+#             */
/*   Updated: 2019/03/22 15:28:54 by prastoin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_string.h"
#include "asm.h"

#define MSG "Expected one of "

char	*from_int_to_type(size_t type)
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

void	error_severity(size_t severity)
{
	severity == 1 ? write(2, CSI_RED, (sizeof(CSI_RED) - 1)) : write(2, CSI_YELLOW, (sizeof(CSI_YELLOW) - 1));
	severity == 1 ? write(2, "error: ", ft_strlen("error: ")) : write(2, "warning: ", ft_strlen("warning: "));
	write(2, CSI_RESET, (sizeof(CSI_RESET) - 1));
}

void	print_error(uintmax_t severity, t_span begin, t_span end, char *error, char *expected)
{
	size_t	fd;
	char	buffer[4096]; //arevoir la taille pas confiant
	size_t	len;

//	printf("\ndeb = %d:%d fin = %d:%d\n", begin.lines, begin.col, end.lines, end.col);
	error_severity(severity);
	//affichage error
	write(2, CSI_WHITE, (sizeof(CSI_WHITE) - 1));
	write(2, error, ft_strlen(error));
	//affichage de l erreur;
	len = end.col + 100;
	fd = open(begin.file_name, O_RDONLY);
	lseek(fd, begin.offset - begin.col + 1, SEEK_SET);
	read(fd, buffer, len);
	size_t i = 0;
	while (i < len && buffer[i] != '\n')
		i++;
	len = i;
	//conseils
	dprintf(2, CSI_RESET "\n- <%s>:%zu:%zu\n", begin.file_name, begin.lines, begin.col);
	dprintf(2, CSI_BLUE "%4zu │ " CSI_RESET "%.*s\n"  CSI_BLUE "     │ ", begin.lines, len, buffer, begin.file_name);
	i = 0;
	while (i < begin.col - 1)
	{
		if (buffer[i] == '\t')
			dprintf(2, "\t");
		else
			dprintf(2, " ");
		i++;
	}
	if (severity == 1)
		write(2, CSI_RED, (sizeof(CSI_RED) - 1));
	else
		write(2, CSI_YELLOW, (sizeof(CSI_YELLOW) - 1));
	while (i < end.col - 1)
	{
		dprintf(2, "^");
		i++;
	}
	if (expected)
		dprintf(2, " %s\n", expected);
	else
		dprintf(2, "\n");

	write(2, CSI_RESET, (sizeof(CSI_RESET) - 1));
}

void	ex_error(char *str)
{
	write(2, CSI_RED, (sizeof(CSI_RED) - 1));
	ft_putstr_fd(str, 2);
	write(2, CSI_RESET, (sizeof(CSI_RESET) - 1));
	exit(0);
}
