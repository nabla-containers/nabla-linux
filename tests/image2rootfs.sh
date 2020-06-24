#!/bin/bash
# Usage: ./image2rootfs.sh app tag
die() { echo "$*" 1>&2 ; exit 1; }

app=$1 tag=$2
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

# install our "init" file
cp init.sh $mnt/init.sh
cp entropy $mnt/
cp hosts $mnt/etc/hosts

# install musl libc
cp ${MUSL_BUILD}/lib/libc.so $mnt/lib/ld-musl-x86_64.so.1
cp ${MUSL_BUILD}/lib/libc.so $mnt/lib/libc.musl-x86_64.so.1
cp ${MUSL_BUILD}/lib/libc.so $mnt/lib/libc.so

# 2**20 blocks of size 1024 makes 1GB
genext2fs -z -f -q -U -P -b $((2**20)) -d $mnt $app.ext3
rm -rf $mnt
