# Jenkins Docker "Attach" agent — Ubuntu 24.04 + Java 17 + x86_64 MinGW-w64 + GNU libiconv
FROM ubuntu:24.04

LABEL maintainer="Thomas Nilefalk <thomas@alanif.se>"

ENV DEBIAN_FRONTEND=noninteractive \
    TZ=Europe/Stockholm \
    LANG=C.UTF-8 \
    LC_ALL=C.UTF-8 \
    JAVA_TOOL_OPTIONS="-Dfile.encoding=UTF-8"

# TZ
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ >/etc/timezone

# Base: Java 17, git/ssh, buildchain, MinGW-w64 64-bit, NSIS, Cgreen, tools for libiconv-build
RUN apt-get update && apt-get -y upgrade && \
    apt-get install -y --no-install-recommends \
      ca-certificates curl wget unzip xz-utils \
      openjdk-17-jre-headless \
      git openssh-client \
      build-essential make cmake pkg-config gcc g++ cgreen1 \
      gcc-mingw-w64-x86-64 g++-mingw-w64-x86-64 binutils-mingw-w64-x86-64 \
      mingw-w64 mingw-w64-tools cgreen1 libcgreen1-dev \
      nsis zip dos2unix && \
    rm -rf /var/lib/apt/lists/*

# Use POSIX-threads for MinGW
RUN update-alternatives --set x86_64-w64-mingw32-gcc /usr/bin/x86_64-w64-mingw32-gcc-posix && \
    update-alternatives --set x86_64-w64-mingw32-g++ /usr/bin/x86_64-w64-mingw32-g++-posix

# Build and install GNU libiconv 1.18 for x86_64-w64-mingw32
#RUN set -eux; \
#    mkdir -p /tmp/libiconv && cd /tmp/libiconv && \
#    wget -q https://ftp.gnu.org/pub/gnu/libiconv/libiconv-1.18.tar.gz && \
#    tar xf libiconv-1.18.tar.gz && cd libiconv-1.18 && \
#    CC=x86_64-w64-mingw32-gcc AR=x86_64-w64-mingw32-ar RANLIB=x86_64-w64-mingw32-ranlib \
#      ./configure --host=x86_64-w64-mingw32 --build="$(gcc -dumpmachine)" \
#                  --prefix=/usr/local/x86_64-w64-mingw32 && \
#    make -j"$(nproc)" && make install && \
#    cd / && rm -rf /tmp/libiconv

# Prefix for iconv
#ENV ICONV_ROOT=/usr/local/x86_64-w64-mingw32

RUN \
    git clone https://github.com/win-iconv/win-iconv.git && \
    cd win-iconv && \
    x86_64-w64-mingw32-gcc -O2 -Wall -c win_iconv.c -o win_iconv.o && \
    ar rcs libwiniconv.a win_iconv.o && \
    x86_64-w64-mingw32-gcc -shared -o winiconv.dll win_iconv.o -Wl,--out-implib,libwiniconv.dll.a

ENV WINICONV_ROOT = /home/jenkins/win-iconv

# Jenkins-user .ssh-directory
RUN useradd -m -d /home/jenkins -s /bin/bash jenkins && \
    install -d -m 700 -o jenkins -g jenkins /home/jenkins/.ssh

USER jenkins
WORKDIR /home/jenkins

# No CMD/ENTRYPOINT — Docker-plugin (Attach) runs the remoting-jar
