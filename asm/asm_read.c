/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asm_read.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/14 11:34:39 by prastoin          #+#    #+#             */
/*   Updated: 2019/03/20 17:47:34 by prastoin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_string.h"
#include "asm.h"

bool	asm_skip_ws(t_read *rd)
{
	int16_t		c;

	while ((c = io_peek(rd)) == ' ' || c == '\t' || c == '#' || c == '\n')
	{
		rd->index++;
		if (c == '#')
			if (!io_skip(rd , '\n'))
				return (false);
	}
	return (true);
}

bool		asm_read_quoted(t_read *rd, char data[], size_t len)
{
	size_t		i;
	int16_t		c;
	
	i = 0;
	if (!(io_expect(rd, "\"")))
		return (false);
	while ((c = io_peek(rd)) != '"' && c != -1)
	{
		if (c == '\\')
		{
			rd->index++;
			if ((c = io_peek(rd)) == -1)
				return (false);
		}
		data[i] = c;
		rd->index++;
		i++;
		if (i >= len)
		{
			io_skip(rd, '"');
			return (false);
		}
	}
	if (i < len)
		data[i] = '\0';
	if (c == '"')
		rd->index++;
	return (c != -1);
}

/*bool		asm_fill_header(t_write *out, t_header *head)
{
	size_t i;

	i = 0;
	asm_fill_magic();
	while (head->name[i] || i < PROG_NAME_LENGTH)
	{
		out->buffer[out->index] = head->name[i];
		out->index++;
		i++;
	}
	return (true);
}*/

size_t		asm_opcode_for(char *name)
{
	size_t i;

	i = 1;
	while (i < 17 && ft_strcmp(name, g_ops[i].name) != 0)
		i++;
	return (i < 17 ? i : -1);
}

bool		asm_parse_header(t_read *rd, t_header *header)
{
	header->size = 0;
	asm_skip_ws(rd);
	io_expect(rd, ".name");
	asm_skip_ws(rd);
	asm_read_quoted(rd, header->name, sizeof(header->name));
	asm_skip_ws(rd);
	io_expect(rd, ".comment");
	asm_skip_ws(rd);
	asm_read_quoted(rd, header->comment, sizeof(header->comment));
	return (true);
}

#include <string.h>

/*bool		asm_parse_op(t_read *in, size_t opcode)
{
	bool	matched[17];
	int16_t	c;
	size_t	i;
	size_t	j;

	memset(matched, true, 17);
	j = 0;
	while ((c = io_peek(in)))
	{
		i = 0;
		while (i < sizeof(matched))
		{
			if (matched[i])
			{
				if (g_ops[i].name[j] == '\0')
				{

				}
				if (g_ops[i].name[j] != c)
					matched[i] = false;
			}
			i++;
		}
		j++;
	}
}*/

char		*parse_label(t_read *in)
{
	size_t		i;
	size_t		len;
	int16_t		c;
	char		*str;

	len = 0;
	if (!(str = (char*)malloc(sizeof(char) * 1)))
		return (NULL);
	while ((c = io_peek(in)) != -1 && ((c >= 'a' && c <= 'b') || (c >= '0' && c <= '9') || c == '_'))
	{
		str[len] = c;
		in->index++;
		len++;
		if (!(str = realloc(str, sizeof(char) * (len + 1))))
			return (NULL);
	}
	str[len] = '\0';
	return (str);
}

bool		asm_parse_params(t_read *in, t_instruction *inst)
{
	size_t		i;
	uint16_t	c;
	char		tmp[2];

	i = 0;
	tmp[0] = SEPARATOR_CHAR;
	tmp[1] = '\0';
	while (g_ops[inst->opcode].params[i])
	{
		asm_skip_ws(in);
		c = io_peek(in);
		in->index++;
		if (c == DIRECT_CHAR)
		{
			inst->params[i].type = PARAM_DIRECT;
			c = io_peek(in);
			if (c == LABEL_CHAR)
			{
				in->index++;
				inst->params[i].direct.label = parse_label(in);
			}
			else
				inst->params[i].direct.offset = io_readnum(in);
		}
		else if (c == 'r')
		{
			inst->params[i].type = PARAM_REGISTER;
			inst->params[i].reg.reg = io_readnum(in);
		}
		else if (c == LABEL_CHAR || (c >= '0' && c <= '9')
				|| (c == '+' || c == '-'))
		{
			if (c != LABEL_CHAR)
				in->index--;
			inst->params[i].type = PARAM_INDIRECT;
			if (c == LABEL_CHAR)
			{
				in->index++;
				inst->params[i].indirect.label = parse_label(in);
			}
			else
				inst->params[i].indirect.offset = io_readnum(in);
		}
		if (!(g_ops[inst->opcode].params[i] & inst->params[i].type))
		{
			// Echo pabon
		}
		i++;
		if (g_ops[inst->opcode].params[i])
		{
			asm_skip_ws(in);
			io_expect(in, tmp);
		}
	}
	return (true);
}

bool		asm_parse_instruction(t_read *in, t_instruction *inst)
{
	char	*tmp;
	char	c;
	size_t	i;

	i = 0;
	asm_skip_ws(in);
	if (!(tmp = (char *)malloc(1)))
		return (false);
	while ((c = io_peek(in)) != -1 && c != ' ' && c != '\t'
			&& c != '\n' && c != ':')
	{
		tmp[i] = c;
		i++;
		in->index++;
		if (!(tmp = realloc(tmp, i + 1)))
			return (false);
	}
	if (c == -1)
		return (false);
	tmp[i] = '\0';
	if (c == ':')
	{
		inst->label = tmp;
		in->index++;
	}
	else
	{
		inst->label = NULL;
		if ((inst->opcode = asm_opcode_for(tmp)) != -1)
			asm_parse_params(in, inst);
		free(tmp);
	}
	return (true);
}

int main(int argc, const char *argv[])
{
	t_read			in;
	t_header		head;
	t_write			out;
	t_instruction	inst;
	t_hashtable		*table;
	t_entry			*entry;

	table = create_hashtable(8);
	in = init_read(open(argv[1], O_RDONLY));
	out = init_write(open("yolo.cor", O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR));
	asm_parse_header(&in, &head);
	write_header(&head, &out);
	while (io_peek(&in) != -1)
	{
		if (!asm_parse_instruction(&in, &inst))
			break ;
		if (inst.label)
			if ((entry = insert_hashtable(&table, create_entry(inst.label))))
			{
				// TODO resolve labels
				entry->value = out.nbr_write;
				printf("Label %s value %llu\n", entry->key, entry->value);
			}
			else
			{
				printf("Label alredy exist: %s\n", inst.label);
			}
		else if (inst.opcode != -1)
			bin_write_inst(&out, &inst);
	}
	bin_write_end(&out, &head);
	printf("lol: %llu\n", hashtable_get(table, "lol")->value);
	printf("lal: %llu\n", hashtable_get(table, "lal")->value);
	return (0);
}
