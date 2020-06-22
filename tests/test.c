#include "syscall_nr.h"

static unsigned long __sysinfo = 0;
#include "syscall_arch.h"

/* direct syscall to the host for debugging purposes */
static void printmsg(char *msg, int len){
	register int    syscall_no  asm("rax") = 1;
	register int    arg1        asm("rdi") = 1;
	register char*  arg2        asm("rsi") = msg;
	register int    arg3        asm("rdx") = len;
	asm("syscall");
}

static inline char *num_to_str(unsigned long num, char *buf, int len)
{
	unsigned int digit;

	/* put digits in buffer from back to front */
	buf += len - 1;
	*buf = 0;
	do {
		digit = num % 10;
		*(--buf) = digit + '0';
		num /= 10;
	} while (num > 0);

	return buf;
}

static int print_num(unsigned long num)
{
	char num_buf[30];

	printmsg(num_to_str(num, num_buf, sizeof(num_buf)), 30);
}

static char msg_global[] = "HELLO\n\r";

static void _Exit(int ec)
{
	//__syscall1(__NR_exit_group, ec);
        for (;;) __syscall1(__NR_exit, ec);
}

static long __syscall_ret(unsigned long r)
{
        if (r > -4096UL) {
                return -1;
        }
        return r;
}

static int vfork()
{
	/* This is exactly the same as in musl */
	__asm__ __volatile__ ("popq %%rcx; call *%0; pushq %%rcx; movq %%rax,%%rdi; jmp __syscall_ret"
                       :: "r"(__sysinfo), "a"(__NR_vfork) : "rcx", "r11", "memory", "rdi");
}

#define	O_RDONLY	0x0000		/* open for reading only */
#define	O_WRONLY	0x0001		/* open for writing only */
#define	O_RDWR		0x0002		/* open for reading and writing */
#define	O_ACCMODE	0x0003		/* mask for above modes */
#define	O_NONBLOCK	0x0004		/* no delay */
#define	O_APPEND	0x0008		/* set append mode */
#define	O_CREAT		0x0200		/* create if nonexistant */

struct timespec {
	long tv_sec;        /* seconds */
	long   tv_nsec;       /* nanoseconds */
};

#define NULL (0)

static void test_files()
{
	char buf[128];
	int fd = __syscall3(__NR_open, "/files/mifile2", O_RDONLY, 0);
	__syscall3(__NR_read, fd, buf, 50);
	__syscall3(__NR_write, 1, buf, 50);
}

static void test()
{
	char msg[] = "hello\n\r";
	int ret = 0;

	printmsg("\n", 1);
	printmsg("\n", 1);
	printmsg(msg, 7);

	struct timespec t;
	t.tv_sec = 0;
	t.tv_nsec = 100;
	__syscall2(__NR_nanosleep, &t, 0);

	__syscall3(__NR_write, 1, msg, 7);

	__syscall3(__NR_write, 1, "hola XXXXX\n\r", 12);

	//__syscall3(__NR_execve, "/noop", NULL, NULL);

	ret = vfork();
	if (ret == 0) {
		__syscall3(__NR_write, 1, "hola child\n\r", 12);
		__syscall3(__NR_write, 1, msg, 7);
		__syscall3(__NR_execve, "/noop", NULL, NULL);

		// should not be executed
		_Exit(0);
	} else if (ret > 0) {
		int status;
		__syscall4(__NR_wait4, ret, &status, 0, 0);
		test_files();
		__syscall3(__NR_write, 1, "hola  dad\n\r", 12);
		_Exit(0);
	} else {
		__syscall3(__NR_write, 1, "error xxx\n\r", 12);
		_Exit(0);
	}
}

typedef unsigned long size_t;

#define AUX_CNT 64
#define DYN_CNT 32

void _start_c(size_t *sp, size_t *dynv)
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

        for (i=0; i<DYN_CNT; i++) dyn[i] = 0;
        for (i=0; dynv[i]; i+=2) if (dynv[i]<DYN_CNT)
                dyn[dynv[i]] = dynv[i+1];

	__sysinfo = aux[AT_SYSINFO];
	base = aux[AT_BASE];

	test();
	_Exit(0);
	return;
}

void _start()
{
	__asm__ __volatile__("movq %rsp,%rdi; .weak _DYNAMIC; .hidden _DYNAMIC; lea _DYNAMIC(%rip),%rsi; andq $(~0 << 4),%rsp; jmp _start_c");
}
