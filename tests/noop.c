#include "syscall_nr.h"

static unsigned long __sysinfo = 0;
#include "syscall-x86_64.h"

typedef unsigned long size_t;

#define AUX_CNT 64
#define DYN_CNT 32

#define AT_NULL         0
#define AT_SYSINFO      32

void _start_c(size_t *sp)
{
        size_t i, aux[AUX_CNT], dyn[DYN_CNT];
        size_t *rel, rel_size, base;

        int argc = *sp;
        char **argv = (void *)(sp+1);

        for (i=argc+1; argv[i]; i++);
        size_t *auxv = (void *)(argv+i+1);

        for (i=0; i<AUX_CNT; i++) aux[i] = 0;
        for (i=0; auxv[i]; i+=2) if (auxv[i]<AUX_CNT)
                aux[auxv[i]] = auxv[i+1];

	__sysinfo = aux[AT_SYSINFO];

	__syscall3(__NR_write, 1, "hola execd\r\n", 12);
        __syscall1(__NR_exit, 0);
}

void _start()
{
	__asm__ __volatile__("movq %rsp,%rdi; andq $(~0 << 4),%rsp; jmp _start_c");
}
