all: linux libc busybox

.PHONY: linux
linux:
	cp linux.config linux-um-nommu/.config
	make -C linux-um-nommu ARCH=um -j4

.PHONY: libc
libc:
	(cd musl-libc ; ./configure --prefix=`pwd`/build --exec-prefix=`pwd`/build)
	make install -j$(nproc) -C musl-libc

.PHONY: busybox
busybox: libc
	cp busybox.config busybox/.config
	CC=../musl-libc/build/bin/musl-gcc make -j$(nproc) -C busybox

clean:
	make -C linux-um-nommu clean
	make -C musl-libc clean
	make -C busybox clean
