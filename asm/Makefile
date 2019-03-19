# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: prastoin <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/01/07 11:13:09 by prastoin          #+#    #+#              #
#    Updated: 2019/03/19 10:28:46 by prastoin         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = asm

NAMES = flag

include srcs.mk

OBJ = $(SRC:.c=.o)

FLAG = -Wall -Wextra -Werror -g -fsanitize=address

INC = -Iincludes

all: $(NAME)

$(OBJ) : Makefile srcs.mk

$(NAME): $(OBJ)
		rm -f *.cor
		gcc $(INC) -o $(NAME) $(OBJ)

$(NAMES): re $(OBJ)
		gcc $(FLAG) $(INC) -o $(NAME) $(OBJ)

%.o: %.c
		gcc $(INC) -o $@ -c $<

clean:
			rm -rf $(OBJ)

fclean: clean
			rm -rf $(NAME)

re: fclean all

.PHONY: clean fclean re all
