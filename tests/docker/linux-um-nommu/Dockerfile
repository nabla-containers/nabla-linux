FROM alpine

RUN mkdir /nabla
ADD vmlinux /nabla/.
ADD alpine-test.ext3 /nabla/.
ADD ignore_winch.sh /nabla/.

CMD cd /nabla && ./ignore_winch.sh ./vmlinux rw ubd0=layer,./alpine-test.ext3 mem=1024m loglevel=10 init=/init.sh sh
