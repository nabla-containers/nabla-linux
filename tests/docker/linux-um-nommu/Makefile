all: build

alpine-test.ext3: ../../alpine-test.ext3
	cp $< $@

vmlinux: ../../../linux-um-nommu/vmlinux
	cp $< $@

build: vmlinux alpine-test.ext3
	sudo docker build -t kollerr/linux-um-nommu .
