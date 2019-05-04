#ifndef EXPORTS_H
# define EXPORTS_H

# ifndef __wasm__
#  error This header should be only used with wasm
# endif

# include "vm.h"

# define export __attribute__((visibility("default")))

export void david_needs_to_work(t_vm *vm);
export t_process *add_process(t_vec **list);

#endif
