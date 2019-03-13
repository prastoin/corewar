/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   op.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/13 16:49:30 by prastoin          #+#    #+#             */
/*   Updated: 2019/03/13 16:54:18 by prastoin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OP_H
#define OP_H

# define IND_SIZE 2
# define REG_SIZE 4
# define DIR_SIZE REG_SIZE


# define REG_CODE 1
# define DIR_CODE 2
# define IND_CODE 3


# define MAX_ARGS_NUMBER 4
# define MAX_PLAYERS 4
# define MEM_SIZE (4*1024)
# define IDX_MOD (MEM_SIZE / 8)
# define CHAMP_MAX_SIZE (MEM_SIZE / 6)

# define COMMENT_CHAR '#'
# define LABEL_CHAR ':'
# define DIRECT_CHAR '%'
# define SEPARATOR_CHAR ','

# define LABEL_CHARS "abcdefghijklmnopqrstuvwxyz_0123456789"

# define NAME_CMD_STRING ".name"
# define COMMENT_CMD_STRING ".comment"

# define REG_NUMBER 16

# define CYCLE_TO_DIE153 6
# define CYCLE_DELTA 50
# define NBR_LIVE 21
# define MAX_CHECKS 10

typedef char t_arg_type;

# define T_REG1
# define T_DIR2
# define T_IND4
# define T_LAB8

# define PROG_NAME_LENGTH (128)
# define COMMENT_LENGTH (2048)
# define COREWAR_EXEC_MAGIC 0xea83f3

typedef struct header_s
{
	unsigned intmagic;
	char prog_name[PROG_NAME_LENGTH + 1];
	unsigned intprog_size;
	char comment[COMMENT_LENGTH + 1];
}header_t;

#endif
