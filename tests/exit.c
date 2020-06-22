#include "syscall_nr.h"

static void kill(int pid, int sig) {
	register int    syscall_no  asm("rax") = 62;
	register int    arg1        asm("rdi") = pid;
	register int    arg2        asm("rsi") = sig;
	asm("syscall");
}

void _start()
{
	kill(0, 9);
	return;
}
