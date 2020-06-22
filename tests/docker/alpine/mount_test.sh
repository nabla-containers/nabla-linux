#!/bin/sh

dd if=/dev/zero of=disk count=8192 bs=8192
mkfs.ext3 disk
mount disk /mnt
ls /mnt
cd /mnt
mkdir a
touch a/b
ln a/b a/c
ln -s a/c a/d
chmod -R +wr *
chown -R root:root *
cp -r a aa
tar -c a | gzip > a.tar.gz
cd /
df
