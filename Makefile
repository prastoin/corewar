# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dde-jesu <dde-jesu@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/04/15 10:32:31 by dde-jesu          #+#    #+#              #
#    Updated: 2019/06/05 10:03:14 by dde-jesu         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = gcc
CFLAGS = -Wall -Wextra -Iinclude -flto -O2
LDFLAGS =

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
OBJS_DISASM = $(filter $(OBJS_DIR)/disassembler/%, $(OBJS))

all: corewar  asm disasm

$(OBJS_DIR)/glue.o.wasm: wasm/glue.c Makefile
	@$(PRECOMPILE)
	$(CC) $(CFLAGS) -c -o $@ $<
	@$(POSTCOMPILE)

$(OBJS_DIR)/%.o $(OBJS_DIR)/%.o.wasm: src/%.c Makefile
	@$(PRECOMPILE)
	$(CC) $(CFLAGS) -c -o $@ $<
	@$(POSTCOMPILE)

corewar: $(OBJS_VM) $(OBJS_COMMON)
asm: $(OBJS_ASM) $(OBJS_COMMON)
disasm: $(OBJS_DISASM) $(OBJS_COMMON)

corewar asm disasm:
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

vm.wasm: CC = ${HOME}/.brew/opt/llvm/bin/clang
vm.wasm: CFLAGS += --target=wasm32 -nostdlib -isystem ./wasm/libc -include wasm/exports.h -ffunction-sections -fdata-sections
vm.wasm: LDFLAGS = -Wl,--no-entry -Wl,--allow-undefined -Wl,--export-dynamic -Wl,--import-memory
vm.wasm: $(addsuffix .wasm, $(filter-out %/vm.o %/hooks.o %/hooks_2.o %/pre_start.o %/vm_aff.o, $(OBJS_VM) $(OBJS_COMMON) $(OBJS_DIR)/glue.o))
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^
	wasm-opt -Oz $@ -o $@

clean:
	rm -rf build

fclean: clean
	rm -rf corewar asm vm.wasm disasm

re:
	$(MAKE) fclean
	$(MAKE) all

tests/node_modules: tests/package-lock.json
	cd tests; npm ci

test: tests/node_modules asm corewar
	npm -C tests run test -- --verbose

include $(wildcard $(DEPS_DIR)/**/*.d)

.PHONY: all clean fclean re test
