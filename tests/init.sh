#!/bin/sh

mount proc /proc -t proc
/sbin/ifconfig lo 127.0.0.1 up
/sbin/ifconfig eth0 10.0.0.2 up
/sbin/ip route add default via 10.0.0.1 dev eth0
/entropy 100000

export PATH=/home:/sbin:/usr/sbin:/bin:/usr/bin

# hanging
#export PS1="\[$(tput bold)\]\[$(tput setaf 4)\][\[$(tput setaf 5)\]\u\[$(tput setaf 4)\]@\[$(tput setaf 5)\]\h \[$(tput setaf 2)\]\W\[$(tput setaf 4)\]]\\$ \[$(tput sgr0)\]"

exec "$@"
