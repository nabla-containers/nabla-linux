#ifndef SYSCALL_ARCH_H
#define SYSCALL_ARCH_H

#define __SYSCALL_LL_E(x) (x)
#define __SYSCALL_LL_O(x) (x)

#define AT_NULL         0
#define AT_IGNORE       1
#define AT_EXECFD       2
#define AT_PHDR         3
#define AT_PHENT        4
#define AT_PHNUM        5
#define AT_PAGESZ       6
#define AT_BASE         7
#define AT_FLAGS        8
#define AT_ENTRY        9
#define AT_NOTELF       10
#define AT_UID          11
#define AT_EUID         12
#define AT_GID          13
#define AT_EGID         14
#define AT_CLKTCK       17
#define AT_PLATFORM     15
#define AT_HWCAP        16
#define AT_FPUCW        18
#define AT_DCACHEBSIZE  19
#define AT_ICACHEBSIZE  20
#define AT_UCACHEBSIZE  21
#define AT_IGNOREPPC    22
#define AT_SECURE       23
#define AT_BASE_PLATFORM 24
#define AT_RANDOM       25
#define AT_HWCAP2       26
#define AT_EXECFN       31
#define AT_SYSINFO      32
#define AT_SYSINFO_EHDR 33

#define KML

#ifdef KML
extern unsigned long __sysinfo;
#endif

static __inline long __syscall0(long n)
{
	unsigned long ret = -1;
#ifdef KML
    if (__sysinfo) {
        __asm__ __volatile__ ("call *%1" : "=a"(ret) : "r"(__sysinfo), "a"(n) : "rcx", "r11", "memory");
        return ret;
    }
#endif
	__asm__ __volatile__ ("syscall" : "=a"(ret) : "a"(n) : "rcx", "r11", "memory");
	return ret;
}

static __inline long __syscall1(long n, long a1)
{
	unsigned long ret = -1;
#ifdef KML
    if (__sysinfo) {
        __asm__ __volatile__ ("call *%1" : "=a"(ret) : "r"(__sysinfo), "a"(n), "D"(a1) : "rcx", "r11", "memory");
        return ret;
    }
#endif   
	__asm__ __volatile__ ("syscall" : "=a"(ret) : "a"(n), "D"(a1) : "rcx", "r11", "memory");
	return ret;
}

static __inline long __syscall2(long n, long a1, long a2)
{
	unsigned long ret = -1;
#ifdef KML
    if (__sysinfo) {
        __asm__ __volatile__ ("call *%1" : "=a"(ret) : "r"(__sysinfo), "a"(n), "D"(a1), "S"(a2)
                              : "rcx", "r11", "memory");
        return ret;
    }
#endif
	__asm__ __volatile__ ("syscall" : "=a"(ret) : "a"(n), "D"(a1), "S"(a2)
						  : "rcx", "r11", "memory");
	return ret;
}

static __inline long __syscall3(long n, long a1, long a2, long a3)
{
	unsigned long ret = -1;
#ifdef KML
    if (__sysinfo) {
        __asm__ __volatile__ ("call *%1" : "=a"(ret) : "r"(__sysinfo), "a"(n), "D"(a1), "S"(a2),
                              "d"(a3) : "rcx", "r11", "memory");
        return ret;
    }
#endif
	__asm__ __volatile__ ("syscall" : "=a"(ret) : "a"(n), "D"(a1), "S"(a2),
						  "d"(a3) : "rcx", "r11", "memory");
	return ret;
}

static __inline long __syscall4(long n, long a1, long a2, long a3, long a4)
{
	unsigned long ret = -1;
#ifdef KML
    if (__sysinfo) {
        register long r10 __asm__("r10") = a4;
        __asm__ __volatile__ ("call *%1" : "=a"(ret) : "r"(__sysinfo), "a"(n), "D"(a1), "S"(a2),
                              "d"(a3), "r"(r10): "rcx", "r11", "memory");
        return ret;
    }
#endif    
	register long r10 __asm__("r10") = a4;
	__asm__ __volatile__ ("syscall" : "=a"(ret) : "a"(n), "D"(a1), "S"(a2),
						  "d"(a3), "r"(r10): "rcx", "r11", "memory");
	return ret;
}

static __inline long __syscall5(long n, long a1, long a2, long a3, long a4, long a5)
{
	unsigned long ret = -1;
#ifdef KML
    if (__sysinfo) {
        register long r10 __asm__("r10") = a4;
        register long r8 __asm__("r8") = a5;
        __asm__ __volatile__ ("call *%1" : "=a"(ret) : "r"(__sysinfo), "a"(n), "D"(a1), "S"(a2),
                              "d"(a3), "r"(r10), "r"(r8) : "rcx", "r11", "memory");
        return ret;
    }
#endif
	register long r10 __asm__("r10") = a4;
	register long r8 __asm__("r8") = a5;
	__asm__ __volatile__ ("syscall" : "=a"(ret) : "a"(n), "D"(a1), "S"(a2),
						  "d"(a3), "r"(r10), "r"(r8) : "rcx", "r11", "memory");
	return ret;
}

static __inline long __syscall6(long n, long a1, long a2, long a3, long a4, long a5, long a6)
{
	unsigned long ret = -1;
#ifdef KML
    if (__sysinfo) {
        register long r10 __asm__("r10") = a4;
        register long r8 __asm__("r8") = a5;
        register long r9 __asm__("r9") = a6;
        __asm__ __volatile__ ("call *%1" : "=a"(ret) : "r"(__sysinfo), "a"(n), "D"(a1), "S"(a2),
                              "d"(a3), "r"(r10), "r"(r8), "r"(r9) : "rcx", "r11", "memory");
        return ret;
    }
#endif
	register long r10 __asm__("r10") = a4;
	register long r8 __asm__("r8") = a5;
	register long r9 __asm__("r9") = a6;
	__asm__ __volatile__ ("syscall" : "=a"(ret) : "a"(n), "D"(a1), "S"(a2),
						  "d"(a3), "r"(r10), "r"(r8), "r"(r9) : "rcx", "r11", "memory");
	return ret;
}

#define VDSO_USEFUL
#define VDSO_CGT_SYM "__vdso_clock_gettime"
#define VDSO_CGT_VER "LINUX_2.6"
#define VDSO_GETCPU_SYM "__vdso_getcpu"
#define VDSO_GETCPU_VER "LINUX_2.6"

#define IPC_64 0

#endif
