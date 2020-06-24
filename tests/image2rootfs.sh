#!/bin/bash
# Usage: ./image2rootfs.sh app tag fs
die() { echo "$*" 1>&2 ; exit 1; }

app=$1 tag=$2
fs=${3:-ext3}
echo $app $tag

container_id=$(docker create $app:$tag || die "run container failed.")
if [ "$container_id" == "" ]; then
    die "empty container id."
fi

app=$(basename $app)
docker export $container_id > $app.tar || die "failed to create tar."
trap "rm $app.tar" EXIT
docker rm $container_id;
mnt=$(mktemp -d)
tar -xf $app.tar -C $mnt

# install devices
mknod -m 666 $mnt/dev/null c 1 3
mknod -m 666 $mnt/dev/zero c 1 5
mknod -m 666 $mnt/dev/ptmx c 5 2
mknod -m 666 $mnt/dev/tty c 5 0
mknod -m 444 $mnt/dev/random c 1 8
mknod -m 444 $mnt/dev/urandom c 1 9

# replace busybox with the nommu busybox
cp ${BUSYBOX_BUILD}/busybox $mnt/bin/busybox
md5sum $mnt/bin/busybox

# install our "init" file
cp init.sh $mnt/init.sh
cp entropy $mnt/
cp hosts $mnt/etc/hosts

# install musl libc
cp ${MUSL_BUILD}/lib/libc.so $mnt/lib/ld-musl-x86_64.so.1
cp ${MUSL_BUILD}/lib/libc.so $mnt/lib/libc.musl-x86_64.so.1
cp ${MUSL_BUILD}/lib/libc.so $mnt/lib/libc.so
md5sum $mnt/lib/libc.so

case "$fs" in
	ext2)
		genext2fs -z -f -q -U -P -b $((2**20)) -d $mnt $app.$fs
		;;
	ext3)
		genext2fs -z -f -q -U -P -b $((2**20)) -d $mnt $app.$fs
		tune2fs -j $app.$fs
		;;
	ext4)
		genext2fs -z -f -q -U -P -b $((2**20)) -d $mnt $app.$fs
		tune2fs -j $app.$fs
		tune2fs -O extent,uninit_bg,dir_index $app.$fs
		;;
	iso)
		genisoimage -o $app.$fs -r $mnt
		;;
	*)
		echo "Not processed"
		;;
esac

rm -rf $mnt
