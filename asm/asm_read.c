/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asm_read.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/14 11:34:39 by prastoin          #+#    #+#             */
/*   Updated: 2019/03/27 11:20:23 by fbecerri         ###   ########.fr       */
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

ssize_t		asm_opcode_for(char *name)
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

char		*asm_parse_name(t_read *in)
{
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

void		asm_read_offset_value(t_read *in, t_param *param)
{
	uint16_t	c;

	c = io_peek(in);
	if (c == LABEL_CHAR)
	{
		io_next(in);
		param->offset.label = asm_parse_name(in);
	}
	else
	{
		param->offset.offset = io_readnum(in);
		param->offset.label = NULL;
	}
}

bool		asm_parse_params(t_read *in, t_instruction *inst)
{
	size_t		i;
	uint16_t	c;

	i = 0;
	while (g_ops[inst->opcode].params[i])
	{
		asm_skip_ws(in);
		in->begin = in->span;
		c = io_peek(in);
		if (c == DIRECT_CHAR)
		{
			io_next(in);
			inst->params[i].type = PARAM_DIRECT;
			asm_read_offset_value(in, inst->params + i);
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
			asm_read_offset_value(in, inst->params + i);
		}
		else
		{
			io_skip_until(in, SEPARATOR_CHAR);
			print_error(1, in->begin, in->span, "Invalid param", from_int_to_type(g_ops[inst->opcode].params[i]));
		}

		if (!(g_ops[inst->opcode].params[i] & inst->params[i].type))
		{
			print_error(2, in->begin, in->span, "Type for param is invalid", from_int_to_type(g_ops[inst->opcode].params[i]));
		}
		i++;
		if (g_ops[inst->opcode].params[i])
		{
			in->begin = in->span;
			asm_skip_ws(in);
			if (!io_expect(in, SEPARATOR_CHAR))
			{
				print_error(1, in->begin, in->span, "Expected " SEPARATOR_CHAR, NULL);
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
	const t_span begin = in->span;

	i = 0;
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

bool		parse_args(int argc, char **argv, bool *streaming, char **file)
{

	*streaming = false;
	if (argc > 3 && argc <= 1)
		return (false);
	if (argc == 3)
		if (ft_strcmp(argv[1], "-s") == 0)
		{
			*file = argv[2];
			*streaming = true;
		}
		else
			return (false);
	else
		*file = argv[1];
	return (true);
}

static char	*ft_strrchr(const char *s, int c)
{
	size_t	len;

	len = ft_strlen(s) + 1;
	while (--len)
		if (s[len] == c)
			return ((char *)s + len);
	return (*s == c ? (char *)s : NULL);
}

char			*change_ext(char *name)
{
	static char	file[PATH_MAX - 1];
	char		*dot;

	dot = ft_strrchr(name, '.');
	if ((dot - name + (sizeof(EXT) - 1)) > PATH_MAX)
		return (NULL);
	ft_memcpy(file, name, dot - name);
	ft_memcpy(file + (dot - name), EXT, sizeof(EXT));
	return (file);
}

void		gest_arg(t_instruction inst, t_hashtable **table, t_write *out, t_read *in)
{
	size_t i;
	size_t last_label;
	t_entry *entry;

	i = 0;
	last_label = 0;
	while (g_ops[inst.opcode].params[i])
	{
		in->begin = in->span;
		if ((inst.params[i].type == PARAM_DIRECT || inst.params[i].type == PARAM_INDIRECT)
				&& inst.params[i].offset.label)
		{
			if ((entry = hashtable_get((*table), inst.params[i].offset.label)))
			{
				inst.params[i].offset.offset = entry->offset;
				if (!entry->resolve)
					entry->offset = out->nbr_write;
				else
					inst.params[i].offset.offset -= (ssize_t)out->nbr_write;
			}
			else
			{
				entry = insert_hashtable(table, create_entry(inst.params[i].offset.label));
				entry->resolve = false;
				entry->offset = out->nbr_write;
				inst.params[i].offset.offset = 0;
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
	bin_write_inst(out, &inst, last_label);
}

void		case_label(t_hashtable **table, t_instruction inst, t_write *out, t_read *in)
{
	t_entry *entry;

	if ((entry = insert_hashtable(table, create_entry(inst.label))))
	{
		entry->offset = out->nbr_write;
		entry->resolve = true;
	}
	else
	{
		entry = hashtable_get((*table), inst.label);
		if (entry->resolve)
			print_error(2, in->begin, in->span, "Label already exists: ", NULL);
		else
		{
			bin_resolve_label(out, entry->offset);
			entry->resolve = true;
			entry->offset = out->nbr_write;
		}
	}
}

bool	ft_header(t_write *out, t_read *in)
{
	t_header		head;

	head = (t_header) {
		.size = 0
	};
	if (!asm_parse_header(in, &head))
		return (false);
	write_header(&head, out);
	return (true);
}

bool	asm_parser(t_write *out, t_read *in, t_hashtable *table)
{
	t_instruction	inst;

	ft_header(out, in);
	in->begin = in->span;
	while (io_peek(in) != -1)
	{
		asm_skip_ws(in);
		in->begin = in->span;
		if (!asm_parse_instruction(in, &inst))
			break ;
		if (inst.label)
			case_label(&table, inst, out, in);
		else if (inst.opcode != -1)
			gest_arg(inst, &table, out, in);
		else
		{
			// TODO gestion de l'erreur
		}
	}
	bin_write_end(out);
	return (true);
}

void		read_fixed(t_read *in, char *name)
{
	uint8_t	buffer[CHAMP_MAX_SIZE + HEADER_SIZE];
	t_write	out;
	t_hashtable		*table;

	out = init_write();
	out.buffer_size = CHAMP_MAX_SIZE + HEADER_SIZE;
	out.buffer = buffer;
	table = create_hashtable(8);
	if (!asm_parser(&out, in, table))
		return ;
	out.fd = open(name, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
	write (out.fd, out.buffer, out.nbr_write);
	printf("fin\n");
	return ;
}

void		read_streaming(t_read *in, char *name)
{
	uint8_t		buffer[BUFFER_SIZE];
	t_write		out;
	t_hashtable		*table;

	out = init_write();
	out.buffer_size = BUFFER_SIZE;
	out.flushable = true;
	out.buffer = buffer;
	out.fd = open(name, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
	table = create_hashtable(8);
	asm_parser(&out, in, table);
	printf("fin\n");
}

int main(int argc, char *argv[])
{
	bool	streaming;
	char	*file;
	char	*out;
	int		fd;
	t_read	in;

	if (!parse_args(argc, argv, &streaming, &file))
	{
		printf("parse_args\n");
		return 1;
		// ERROR
	}
	if (!(out = change_ext(file)))
	{
		printf("change_ext\n");
		return 1;
		// ERROR
	}
	if ((fd = open(file, O_RDONLY)) == -1)
	{
		printf("open failed\n");
		return 1;
		// ERROR
	}
	in = init_read(fd, file);
	(streaming ? read_streaming : read_fixed)(&in, out);
}
