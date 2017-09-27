FROM debian:jessie 

RUN apt-get update && apt-get -y install locales ssh
RUN apt-get -y install gcc 
RUN apt-get -y install build-essential
RUN apt-get -y install nasm
RUN apt-get -y install qemu
RUN export LANG=C.UTF-8

VOLUME ["/root/dumberOS"]

CMD ["bash"]