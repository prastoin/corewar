/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asm_read.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/14 11:34:39 by prastoin          #+#    #+#             */
/*   Updated: 2019/03/26 10:49:36 by prastoin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_string.h"
#include "asm.h"

bool	asm_skip_ws(t_read *rd)
{
	int16_t		c;

	while ((c = io_peek(rd)) == ' ' || c == '\t' || c == '#' || c == '\n')
	{
		io_next(rd);
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
	while ((c = io_peek(rd)) != '"' && c != '\n' && c != -1)
	{
		if (c == '\\')
		{
			io_next(rd);
			if ((c = io_peek(rd)) == -1)
				return (false);
		}
		data[i] = c;
		io_next(rd);
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
		io_next(rd);
	return (c == '"');
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
	t_span begin;
	
	header->size = 0;
	asm_skip_ws(rd);
	begin = rd->span;
	if (io_expect(rd, "."))
	{
		if (!io_expect(rd, "name"))
		{
			io_skip_until(rd, " #\t\n\"");
			print_error(1, begin, rd->span, "Expected \".name\"", "Replace by .name");
		}
		else
		{
			asm_skip_ws(rd);
			begin = rd->span;
			if (!asm_read_quoted(rd, header->name, sizeof(header->name)))
				print_error(1, begin, rd->span, "Unclosed \" for .name", NULL);
		}
	}
	else
	{
		print_error(1, begin, rd->span, ".name not found", NULL);
	}
	asm_skip_ws(rd);
	begin = rd->span;
	if (io_expect(rd, "."))
	{
		if (!io_expect(rd, "comment"))
		{
			io_skip_until(rd, " #\t\n\"");
			print_error(1, begin, rd->span, "Expected \".comment\"", "Replace by .comment");
		}
		else
		{
			asm_skip_ws(rd);
			begin = rd->span;
			if (!asm_read_quoted(rd, header->comment, sizeof(header->comment)))
				print_error(1, begin, rd->span, "Unclosed \" for .comment", NULL);
		}
	}
	else
	{
		print_error(1, begin, rd->span, ".comment not found", NULL);
	}
	return (true);
}

#include <string.h>

/*bool		asm_parse_op(t_read *in, size_t opcode)
  {4444444444
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
		io_next(in);
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
	t_span		begin;

	i = 0;
	while (g_ops[inst->opcode].params[i])
	{
		asm_skip_ws(in);
		begin = in->span;
		c = io_peek(in);
		if (c == DIRECT_CHAR)
		{
			io_next(in);
			inst->params[i].type = PARAM_DIRECT;
			c = io_peek(in);
			if (c == LABEL_CHAR)
			{
				io_next(in);
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
			io_next(in);
			inst->params[i].type = PARAM_REGISTER;
			inst->params[i].reg.reg = io_readnum(in);
		}
		else if (c == LABEL_CHAR || (c >= '0' && c <= '9')
				|| (c == '+' || c == '-'))
		{
			inst->params[i].type = PARAM_INDIRECT;
			if (c == LABEL_CHAR)
			{
				io_next(in);
				inst->params[i].indirect.label = asm_parse_name(in);
			}
			else
			{
				inst->params[i].indirect.offset = io_readnum(in);
				inst->params[i].direct.label = NULL;
			}
		}
		else
		{
			io_skip_until(in, SEPARATOR_CHAR);
			print_error(1, begin, in->span, "Invalid param", from_int_to_type(g_ops[inst->opcode].params[i]));
		}
		if (!(g_ops[inst->opcode].params[i] & inst->params[i].type))
		{
			print_error(2, begin, in->span, "Type for param is invalid", from_int_to_type(g_ops[inst->opcode].params[i]));
		}
		i++;
		if (g_ops[inst->opcode].params[i])
		{
			begin = in->span;
			asm_skip_ws(in);
			if (!io_expect(in, SEPARATOR_CHAR))
			{
				print_error(1, begin, in->span, "Expected " SEPARATOR_CHAR, NULL);
			}
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
	const t_span begin = in->span;
	if (!(tmp = asm_parse_name(in)))
		return (false);
	c = io_peek(in); //TODO protect return
	if (c == ':')
	{
		inst->label = tmp;
		io_next(in);
	}
	else
	{
		inst->label = NULL;
		if ((inst->opcode = asm_opcode_for(tmp)) != -1)
			asm_parse_params(in, inst);
		else
		{
			io_skip_until(in, " \t\n#");
			print_error(1, begin, in->span, "Unknown Instructions", NULL);
		}
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
	uint8_t			last_label;
	t_span			begin;

	table = create_hashtable(8);
	in = init_read(open(argv[1], O_RDONLY), (char *)argv[1]);
	out = init_write(open("yolo.cor", O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR));
	head = (t_header) {
		.size = 0
	};
	if (!asm_parse_header(&in, &head))
		return (0);
	write_header(&head, &out);
	begin = in.span;
	while (io_peek(&in) != -1)
	{
		asm_skip_ws(&in);
		begin = in.span;
		if (!asm_parse_instruction(&in, &inst))
			break ;
		if (inst.label)
			if ((entry = insert_hashtable(&table, create_entry(inst.label))))
			{
				entry->offset = out.nbr_write;
				entry->resolve = true;
			}
			else
			{
				entry = hashtable_get(table, inst.label);
				if (entry->resolve)
					print_error(2, begin, in.span, "Label already exists: ", NULL); //mauvais token en span.col
				else
				{
					bin_resolve_label(&out, entry->offset);
					entry->resolve = true;
					entry->offset = out.nbr_write;
				}
			}
		else if (inst.opcode != -1)
		{
			i = 0;
			last_label = 0;
			while (g_ops[inst.opcode].params[i])
			{
	begin = in.span;
				if (inst.params[i].type == PARAM_DIRECT
						&& inst.params[i].direct.label)
				{
					if ((entry = hashtable_get(table, inst.params[i].direct.label)))
					{
						inst.params[i].direct.offset = entry->offset;
						if (!entry->resolve)
							entry->offset = out.nbr_write;
						else
							inst.params[i].direct.offset -= (ssize_t)out.nbr_write;
					}
					else
					{
						entry = insert_hashtable(&table, create_entry(inst.params[i].direct.label));
						entry->resolve = false;
						entry->offset = out.nbr_write;
						inst.params[i].direct.offset = 0;
					}
					if (!entry->resolve)
					{
						if (last_label == 1)
							last_label = 3;
						else
							last_label = i;
					}
				}
				if (inst.params[i].type == PARAM_INDIRECT
						&& inst.params[i].indirect.label)
				{
					if ((entry = hashtable_get(table, inst.params[i].indirect.label)))
					{
						inst.params[i].indirect.offset = entry->offset;
						if (!entry->resolve)
							entry->offset = out.nbr_write;
						else
							inst.params[i].direct.offset -= (ssize_t)out.nbr_write;
					}
					else
					{
						entry = insert_hashtable(&table, create_entry(inst.params[i].indirect.label));
						entry->resolve = false;
						entry->offset = out.nbr_write;
						inst.params[i].indirect.offset = 0;
					}
					if (!entry->resolve)
					{
						if (last_label == 1)
							last_label = 3;
						else
							last_label = i;
					}
				}
				i++;
			}
			bin_write_inst(&out, &inst, last_label);
		}
	}
	bin_write_end(&out, &head);
	return (0);
}
