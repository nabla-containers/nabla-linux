FROM alpine:3.10

RUN apk update && apk add python py-pip python3 nginx curl file build-base python-dev \
	    fio redis vim clang git libtirpc-dev libtirpc procps imlib2 linux-headers ncurses sl nodejs
#RUN apk update && apk add vim python nginx redis file curl py-pip build-base file python-dev
#RUN apk update && apk add vim python3 nginx redis py-pip build-base file curl
#RUN pip3 install ipython httplib2
COPY bm_tornado_http.py /bm_tornado_http.py
RUN pip install tornado==4.5.3 six numpy==1.15
RUN (git clone https://github.com/ricarkol/lmbench2.git; cd lmbench2; make)

RUN mv /etc/nginx/nginx.conf /etc/nginx/nginx.conf.bak
COPY nginx.conf /etc/nginx/nginx.conf

RUN mv /etc/redis.conf /etc/redis.conf.bak
COPY redis.conf /etc/redis.conf

COPY lmbench_run.sh /lmbench2/bin/x86_64-linux-gnulibc1/lmbench_run.sh

RUN apk add e2fsprogs
COPY mount_test.sh /mount_test.sh

RUN echo 'export PS1="[\u@docker] \W # "' >> /root/.ash_profile
RUN apk add gnuplot
COPY plot.gpi /plot.gpi
