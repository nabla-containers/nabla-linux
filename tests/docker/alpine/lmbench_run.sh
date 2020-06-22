cp hello /tmp/hello

ENOUGH=10000 time ./lat_select file 10
ENOUGH=10000 time ./lat_select file 100
ENOUGH=10000 time ./lat_select file 1000

ENOUGH=100000 time ./lat_syscall null
ENOUGH=100000 time ./lat_syscall read
ENOUGH=100000 time ./lat_syscall write
ENOUGH=100000 time ./lat_syscall stat
ENOUGH=100000 time ./lat_syscall open
ENOUGH=10000 time ./lat_proc shell
ENOUGH=10000 time ./lat_proc exec
