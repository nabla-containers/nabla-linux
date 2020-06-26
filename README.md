This is an experimental type of Virtual Machine which does not use a hypervisor (no monitor, no emulation, no HW virtualization). It's based 
on User-Mode-Linux (UML) and no-MMU.

# Try it with this one-liner

```
docker run --rm -it kollerr/linux-um-nommu
```
The source dockerfile is `tests/docker/linux-um-nommu/Dockerfile` which uses the `alpine-test.ext3` target
in `tests/Makefile` (which is then based on `tests/docker/alpine/Dockerfile`).

# Demo:

[![asciicast](https://asciinema.org/a/343173.svg)](https://asciinema.org/a/343173)

This shows a run with the host syscalls on the left. The point of this is to show that lots of applications just
work while running on a small set of syscalls.

# Introduction

Container runtimes have been using virtualization as a way of improving isolation (e.g., Kata containers). And in order to make them feel like regular containers, the community has been trying to slim down their virtual machine (VM) monitors (e.g., Firecracker). This experimental "VM" is what happens when you slim down to the extreme: no monitor at all.

Nabla Linux is a Linux virtual machine that runs as a single unprivileged user-level process on top of only 12 syscalls. We achieve isolation equivalent to virtual machines, without using a monitor, by restricting the VM process to only these 12 system calls using seccomp. The system was built on top of a combination of two well known Linux features: user mode linux (UML) and no-MMU support (used for embedded devices) both in the kernel and in userspace (musl and busybox).

Our initial experiments show that this Linux VM is capable of running multiple unmodified binaries from Alpine (like python, nginx, redis), and can boot in 6 milliseconds (to our knowledge, this is the fastest); albeit with some limitations: PIE executables only, and no forks (processes are emulated using vforks).

# Limitations

- No virtual memory (VM) and no memory protection.
- No forks. Which is partially solved by using vforks instead.
- Can only run PIE executables (https://en.wikipedia.org/wiki/Position-independent_code).
- Have to use our modified musl libc.

# Example: alpine-test.ext3

