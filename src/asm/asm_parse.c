/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asm_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/26 12:46:07 by prastoin          #+#    #+#             */
/*   Updated: 2019/04/30 17:42:51 by dde-jesu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common.h"
#include "ft_string.h"
#include "asm.h"

bool	is_labelchar(char c)
{
	size_t	i;
	size_t	len;
	char	*str;

	str = LABEL_CHARS;
	len = ft_strlen(LABEL_CHARS);
	i = 0;
	while (i < len)
	{
		if (c == str[i])
			return (true);
		i++;
	}
	return (false);
}

char		*asm_get_inst(t_read *in)
{
	size_t		len;
	int16_t		c;
	char		*str;

	len = 0;
	if (!(str = (char*)malloc(sizeof(char) * 1)))
		return (NULL);
	while ((c = io_peek(in)) != -1 && is_labelchar(c)) //pb si on change nom inst pas bon
	{
		str[len] = c;
		io_next(in);
		len++;
		if (!(str = realloc(str, sizeof(char) * (len + 1))))
			return (NULL);
	}
	str[len] = '\0';
	return (str);
}

void	asm_transform(t_write *out, t_read *in)
{
	t_instruction	inst;
	t_hashtable		*table;

	if (!(table = create_hashtable(8)))
		return ((void)print_small_error(in, ERR, "Hashtable creation failed"));
	bin_write_header(asm_read_header(in), out);
	asm_skip_ws(in);
	while (io_peek(in) != -1 && out->fd != -1)
	{
		mark_span(in);
		if (!asm_read_inst(in, &inst))
			break ; // ==> pb de malloc TODO: Afficher
		if (inst.label)
			asm_store_label(&table, inst.label, out, in);
		else if (inst.opcode != -1)
			bin_write_inst(out, &inst, asm_resolve_label(&table, &inst, out, in));
		else
		{
			io_skip_until(in, " \t\n#");
			print_error(in, ERR, "Unknown Instructions", NULL);
		}
		asm_skip_ws(in);
	}
	bin_write_end(out);
}
