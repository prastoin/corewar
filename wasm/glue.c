#include "vm.h"

export const uint32_t	VM_MEM_SIZE = MEM_SIZE;

export t_vm				*create_vm()
{
	static t_vm	vm;

	vm = (t_vm) {
		.cycle_to_die = CYCLE_TO_DIE,
		.flags = {
			.dump_c = -1
		},
		.vec = create_process(MAX_PLAYERS)
	};
	return &vm;
}

export uint8_t			*get_vm_mem(t_vm *vm)
{
	return vm->mem;
}

export t_vec			**get_vm_vec(t_vm *vm)
{
	return &vm->vec;
}

export t_process		*get_vm_vec_processes(t_vm *vm)
{
	return vm->vec->processes;
}

export size_t			get_vm_mem_size()
{
	return MEM_SIZE;
}

export size_t			get_cycle_to_die()
{
	return CYCLE_TO_DIE;
}

export size_t			get_process_size()
{
	return sizeof(t_process);
}

export void				init_process(t_process *self, ssize_t offset, size_t i)
{
	*self = (t_process) {
		.offset = offset,
		.is_alive = true
	};
	conv_int_to_bin(~i, self->registre[0]);
}

void					*memcpy(void *dst, const void *src, size_t n)
{
	const char	*c_src = src;
	char		*c_dst;

	c_dst = dst;
	while (n--)
		*c_dst++ = *c_src++;
	return (dst);
}

void					*memset(void *s, int c, size_t n)
{
	while (n--)
		((unsigned char *)s)[n] = c;
	return (s);
}
