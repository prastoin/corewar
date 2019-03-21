/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asm_read.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/14 11:34:39 by prastoin          #+#    #+#             */
/*   Updated: 2019/03/21 10:40:00 by prastoin         ###   ########.fr       */
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

char		*asm_parse_name(t_read *in)
{
	size_t		i;
	size_t		len;
	int16_t		c;
	char		*str;

	len = 0;
	if (!(str = (char*)malloc(sizeof(char) * 1)))
		return (NULL);
	while ((c = io_peek(in)) != -1 && ((c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '_'))
	{
		str[len] = c;
		in->index++;
		len++;
		if (!(str = realloc(str, sizeof(char) * (len + 1))))
			return (NULL);
	}
	str[len] = '\0';
	if (c == -1)
		return (NULL);
	return (str);
}

bool		asm_parse_params(t_read *in, t_instruction *inst)
{
	size_t		i;
	uint16_t	c;

	i = 0;
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
				inst->params[i].direct.label = asm_parse_name(in);
			}
			else
			{
				inst->params[i].direct.offset = io_readnum(in);
				inst->params[i].direct.label = NULL;
			}
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
				inst->params[i].indirect.label = asm_parse_name(in);
			}
			else
			{
				inst->params[i].indirect.offset = io_readnum(in);
				inst->params[i].direct.label = NULL;
			}
		}
		if (!(g_ops[inst->opcode].params[i] & inst->params[i].type))
		{
			// Echo pabon
		}
		i++;
		if (g_ops[inst->opcode].params[i])
		{
			asm_skip_ws(in);
			io_expect(in, SEPARATOR_CHAR);
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
	if (!(tmp = asm_parse_name(in)))
		return (false);
	c = io_peek(in); //TODO protect return
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
	size_t			i;

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
				entry->offset = out.nbr_write;
				entry->resolve = true;
				printf("Label %s offset %llu\n", entry->key, entry->offset);
			}
			else
			{
				// TODO resolve labels
				entry = hashtable_get(table, inst.label);
				if (entry->resolve)
					printf("Label alredy exist: %s\n", inst.label);
	//			else
	//				bin_resolve_label(out, entry->offset);
			}
		else if (inst.opcode != -1)
		{
			i = 0;
			while (g_ops[inst.opcode].params[i])
			{
				if (inst.params[i].type == PARAM_DIRECT
						&& inst.params[i].direct.label)
				{
					printf("resolving direct %s\n", inst.params[i].direct.label);
					if ((entry = hashtable_get(table, inst.params[i].direct.label)))
					{
						inst.params[i].direct.offset = entry->offset;
						if (!entry->resolve)
							entry->offset = out.nbr_write;
					}
					else
					{
						entry = insert_hashtable(&table, create_entry(inst.params[i].direct.label));
						entry->resolve = false;
						entry->offset = out.nbr_write;
						inst.params[i].direct.offset = 0;
					}
				}
				if (inst.params[i].type == PARAM_INDIRECT
						&& inst.params[i].indirect.label)
				{
					printf("resolving indirect %s\n", inst.params[i].indirect.label);
					if ((entry = hashtable_get(table, inst.params[i].indirect.label)))
					{
						inst.params[i].indirect.offset = entry->offset;
						if (!entry->resolve)
							entry->offset = out.nbr_write;
					}
					else
					{
						entry = insert_hashtable(&table, create_entry(inst.params[i].indirect.label));
						entry->resolve = false;
						entry->offset = out.nbr_write;
						inst.params[i].indirect.offset = 0;
					}
				}
				i++;
			}
			bin_write_inst(&out, &inst);
		}
	}
	bin_write_end(&out, &head);
	return (0);
}
