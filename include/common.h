/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   common.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/26 11:30:59 by prastoin          #+#    #+#             */
/*   Updated: 2019/04/29 16:27:27 by prastoin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMON_H
#define COMMON_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdbool.h>

# define MAX_PARAM 4
# define MAX_PLAYERS 4
# define MEM_SIZE (4*1024)
# define IDX_MOD (MEM_SIZE / 8)
# define CHAMP_MAX_SIZE (MEM_SIZE / 6)

# define COMMENT_CHAR '#'
# define LABEL_CHAR ':'
# define DIRECT_CHAR '%'
# define SEPARATOR_CHAR ","
# define LABEL_CHARS "abcdefghijklmnopqrstuvwxyz_0123456789"
# define NAME_CMD_STRING ".name"
# define COMMENT_CMD_STRING ".comment"

# define REG_NUMBER 16
# define REG_SIZE 4

# define CYCLE_TO_DIE 1536
# define CYCLE_DELTA 50
# define NBR_LIVE 21
# define MAX_CHECKS 10

# define PROG_NAME_LENGTH (128)
# define COMMENT_LENGTH (2048)
# define COREWAR_EXEC_MAGIC 0xea83f3

# define UNKNOWN_OPTION (-1)
# define NO_ARG (-2)
# define INVALID_VALUE (-3)

typedef enum	e_core_param {
	PARAM_NONE = 0b0,
	PARAM_DIRECT = 0b1,
	PARAM_INDIRECT = 0b10,
	PARAM_REGISTER = 0b100,
	PARAM_ALL = 0b111,
	PARAM_INDEX = 0b1000
}				t_core_param;

typedef struct	s_core_tab
{
	size_t			opcode;
	char			*name;
	bool			ocp;
	size_t			cycle;
	t_core_param	params[MAX_PARAM];
}				t_core_tab;

typedef struct	s_param_offset {
	t_core_param	type;
	char			*label;
	int32_t			offset;
}				t_param_offset;

typedef struct	s_param_register {
	t_core_param	type;
	uint8_t			reg;
}				t_param_register;

extern t_core_tab g_ops[17];

/*
 * arg management
 */

typedef enum	e_arg_type
{
	ARG_END = 0,
	ARG_PLAYERS,
	ARG_BOOLEAN,
	ARG_INT
}				t_type;

typedef	struct	s_arg
{
	t_type		type;
	uint8_t		short_name;
	char		*long_name;
	void		*value;
	const char	*help;
}				t_arg;

int		show_err(int err, char *name, char *option, size_t len);

int		ft_putf_fd(int fd, char *fmt, ...);
int		ft_putf(char *fmt, ...);

int		args_usage(const t_arg args[], char *name, char *usage, char *desc);
int		get_value(char **c_arg, const t_arg *opt, char *argv[], int i[2]);
bool	parse_long(const t_arg opt[], char **arg, char *argv[], size_t *i);
bool	parse_short(const t_arg opt[], char **arg, char *argv[], size_t *i);
ssize_t	parse_args(const t_arg args[], int argc, char *argv[]);

#endif
