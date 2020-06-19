

.PHONY: libc
libc:
	(cd musl-libc ; ./configure --prefix=`pwd`/build --exec-prefix=`pwd`/build)
	(cd musl-libc ; make install -j$(nproc))

.PHONY: busybox
busybox:
	cp busybox.config busybox/.config
	CC=../musl-libc/build/bin/musl-gcc make -j$(nproc) -C busybox
