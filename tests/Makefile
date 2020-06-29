all: run

MUSL_BUILD ?= ../musl-libc/build
BUSYBOX_BUILD ?= ../busybox
VMLINUX ?= ../linux-um-nommu/vmlinux

# sudo is needed for mounting in ./image2rootfs_mount.sh. Otherwise, use
# ./image2rootfs.sh which doesn't require privileges and uses genext2fs
# instead.

alpine-test.ext3: init.sh entropy
	sudo bash -c 'BUSYBOX_BUILD=$(BUSYBOX_BUILD) MUSL_BUILD=$(MUSL_BUILD) ./image2rootfs_mount.sh kollerr/alpine-test latest ext3'

alpine-test.iso: init.sh entropy
	sudo bash -c 'BUSYBOX_BUILD=$(BUSYBOX_BUILD) MUSL_BUILD=$(MUSL_BUILD) ./image2rootfs.sh kollerr/alpine-test latest iso'

pytorch-alpine.ext3: init.sh entropy
	sudo bash -c 'BUSYBOX_BUILD=$(BUSYBOX_BUILD) MUSL_BUILD=$(MUSL_BUILD) ./image2rootfs_mount.sh petronetto/pytorch-alpine latest ext3'

alpine-jupyter-notebook.ext3: init.sh entropy
	sudo bash -c 'BUSYBOX_BUILD=$(BUSYBOX_BUILD) MUSL_BUILD=$(MUSL_BUILD) ./image2rootfs_mount.sh kollerr/alpine-jupyter-notebook latest ext3'

# not working as python (from anaconda) is not a PIE executable
tensorflow-alpine.ext3: init.sh entropy
	sudo bash -c 'BUSYBOX_BUILD=$(BUSYBOX_BUILD) MUSL_BUILD=$(MUSL_BUILD) ./image2rootfs_mount.sh bwits/tensorflow-alpine latest ext3'

testpie: test.c
	$(MUSL_BUILD)/bin/musl-gcc -nostdlib -pie -fPIE -fomit-frame-pointer $< -o $@

test: test.c
	$(MUSL_BUILD)/bin/musl-gcc -O0 -ggdb -nostdlib -shared -fPIC -fomit-frame-pointer $< -o $@

exit: exit.c
	$(MUSL_BUILD)/bin/musl-gcc -O0 -ggdb -nostdlib -shared -fPIC -fomit-frame-pointer $< -o $@

exitpie: exit.c
	$(MUSL_BUILD)/bin/musl-gcc -nostdlib -pie -fPIE -fomit-frame-pointer $< -o $@

noop: noop.c
	$(MUSL_BUILD)/bin/musl-gcc -O0 -ggdb -nostdlib -shared -fPIC -fomit-frame-pointer $< -o $@

ctest: ctest.c
	$(MUSL_BUILD)/bin/musl-gcc -pie -fPIE $< -o $@

futex: futex.c
	$(MUSL_BUILD)/bin/musl-gcc -pie -fPIE $< -o $@

clone: clone.c
	$(MUSL_BUILD)/bin/musl-gcc -pie -fPIE $< -o $@

test_libc.iso: test testpie ctest noop exit exitpie futex clone
	mkdir -p /tmp/nl_mnt
	cp test noop testpie exit exitpie ctest futex clone /tmp/nl_mnt/.
	chmod -R 777 /tmp/nl_mnt
	mkdir /tmp/nl_mnt/files
	mkdir /tmp/nl_mnt/lib
	echo "inside uml." | tee /tmp/nl_mnt/files/mifile
	echo "inside uml dos.**********************************" | tee /tmp/nl_mnt/files/mifile2
	cp $(MUSL_BUILD)/lib/libc.so /tmp/nl_mnt/lib/ld-musl-x86_64.so.1
	cp $(MUSL_BUILD)/lib/libc.so /tmp/nl_mnt/lib/libc.so
	genisoimage -o $@ -r /tmp/nl_mnt
	rm -rf /tmp/nl_mnt

entropy: entropy.c
	$(MUSL_BUILD)/bin/musl-gcc entropy.c -pie -fPIE -o entropy

test_as: test.S
	as -s test.S -o test.o
	ld -non_shared -static --pic-executable test.o -o $@

run_tty: alpine-test.ext3
	# show the output (only): "sudo cat /dev/vcs6 > /tmp/foo ; cat /tmp/foo"
	# find out what's the vcs[1-9]
	sudo $(VMLINUX) ubda=./$< rw mem=1024m con0=tty:/dev/tty6 init=/init.sh /bin/sh

run: test_libc.iso
	$(VMLINUX) ubda=./test_libc.iso ro loglevel=0 mem=512m init=/ctest noop

# measure boot time (time to exit through host exit syscall)
boot:
	# install hyperfine (https://github.com/sharkdp/hyperfine.git)
	taskset -c 0 hyperfine -w 10 -r 50 -i '$(VMLINUX) ubd0=./test_libc.iso ro mem=64m loglevel=0 init=/exit'

# measure boot time (time to exit through libc)
boot_libc:
	taskset -c 0 hyperfine -w 10 -r 50 '$(VMLINUX) ubd0=./test_libc.iso ro mem=64m loglevel=0 init=/ctest noop'

demo: alpine-test.ext3
	taskset -c 0 $(VMLINUX) eth0=tuntap,tap100,fe:fd:0:0:0:1,10.0.0.1 ubd0=./$< rw mem=1024m loglevel=0 init=/init.sh /bin/sh

demo2: alpine-test.ext3
	taskset -c 0 $(VMLINUX) rw ubd0=./$< mem=1024m loglevel=0 init=/init.sh /bin/sh

syscalls:
	# don't remove the space before %evt.type
	sudo bash -c "sysdig proc.name=vmlinux and evt.type!='switch' and evt.type!='signaldeliver' and evt.dir='>' --print=' %evt.type' | python histo.py"

clean:
	rm -f alpine-test.ext3 test_libc.iso clone ctest \
	docker/linux-um-nommu/alpine-test.ext3 \
	docker/linux-um-nommu/vmlinux \
	entropy exit exitpie futex noop test test_libc.iso testpie
