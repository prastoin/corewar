# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: prastoin <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/01/07 11:13:09 by prastoin          #+#    #+#              #
#    Updated: 2019/04/08 09:27:15 by prastoin         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAMEASM = asm

NAMEVM = Corewar

NAMES = flag

include srcs.mk

OBJASM = $(ASMSRC:.c=.o)

OBJVM = $(VMSRC:.c=.o)

OBJFT = $(FTSRC:.c=.o)

FLAG = -Wall -Wextra -g -fsanitize=address

INC = -Iincludes/

all: $(NAMEVM) $(NAMEASM)

$(OBJ) : Makefile srcs.mk

$(NAMEASM): $(OBJASM) $(OBJFT)
		gcc $(FLAG) $(INC) -o $(NAMEASM) $(OBJFT) $(OBJASM)

$(NAMEVM): $(OBJVM) $(OBJFT)
		gcc $(FLAG) $(INC) -o $(NAMEVM) $(OBJFT) $(OBJVM)

$(NAMES): re $(OBJ)
		gcc $(FLAG) $(INC) -o $(NAME) $(OBJ)

%.o: %.c
		gcc $(FLAG) $(INC) -o $@ -c $<

clean:
			rm  -rf $(OBJVM)
			rm  -rf $(OBJASM)
			rm  -rf $(OBJFT)

fclean: clean
			rm  -rf $(NAMEVM)
			rm  -rf $(NAMEASM)
			rm  -rf $(OBJFT)

re: fclean all

.PHONY: clean fclean re all
