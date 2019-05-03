# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dde-jesu <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/04/15 10:32:31 by dde-jesu          #+#    #+#              #
#    Updated: 2019/05/01 14:00:26 by fbecerri         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = gcc
CFLAGS = -Wall -Wextra -Iinclude

BUILD_DIR = build
OBJS_DIR = $(BUILD_DIR)/objs
DEPS_DIR = $(BUILD_DIR)/deps

PRECOMPILE = mkdir -p $(dir $@)
POSTCOMPILE = sleep 0

ifndef NODEPS

PRECOMPILE += ;mkdir -p $(dir $(DEPS_DIR)/$*)
CFLAGS += -MT $@ -MMD -MP -MF $(DEPS_DIR)/$*.Td
POSTCOMPILE += ;mv -f $(DEPS_DIR)/$*.Td $(DEPS_DIR)/$*.d && touch $@

endif

include src.mk

OBJS = $(patsubst src/%.c, $(OBJS_DIR)/%.o, $(SRCS))

OBJS_COMMON = $(filter $(OBJS_DIR)/common/%, $(OBJS))
OBJS_VM = $(filter $(OBJS_DIR)/vm/%, $(OBJS))
OBJS_ASM = $(filter $(OBJS_DIR)/asm/%, $(OBJS))

all: vm asm

$(OBJS_DIR)/%.o: src/%.c Makefile
	@$(PRECOMPILE)
	$(CC) $(CFLAGS) -c -o $@ $<
	@$(POSTCOMPILE)

vm: $(OBJS_VM) $(OBJS_COMMON)
asm: $(OBJS_ASM) $(OBJS_COMMON)

vm asm:
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -rf build

fclean: clean
	rm -rf vm asm

re:
	$(MAKE) fclean
	$(MAKE) all

include $(wildcard $(DEPS_DIR)/**/*.d)

.PHONY: all clean fclean re
