# Jenkins Docker "Attach" agent — Ubuntu 24.04 + Java 21 + x86_64 MinGW-w64 + GNU libiconv
FROM ubuntu:24.04

LABEL maintainer="Thomas Nilefalk <thomas@alanif.se>"

ENV DEBIAN_FRONTEND=noninteractive \
    TZ=Europe/Stockholm \
    LANG=C.UTF-8 \
    LC_ALL=C.UTF-8 \
    JAVA_TOOL_OPTIONS="-Dfile.encoding=UTF-8"

# TZ
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ >/etc/timezone

# Base: Java 21, git/ssh, buildchain, MinGW-w64 64/32-bit + dev, NSIS, Cgreen,
# sshpass (bin/alanupload uses it when SSHPASS is set), tools for libiconv-build
RUN apt-get update && apt-get -y upgrade && \
    apt-get install -y --no-install-recommends \
      ca-certificates curl wget unzip xz-utils \
      openjdk-21-jre-headless \
      git openssh-client \
      build-essential make cmake pkg-config gcc g++ cgreen1 \
      gcc-mingw-w64-x86-64 g++-mingw-w64-x86-64 binutils-mingw-w64-x86-64 \
      gcc-mingw-w64-i686 binutils-mingw-w64-i686 mingw-w64-i686-dev \
      mingw-w64 mingw-w64-tools mingw-w64-x86-64-dev \
      cgreen1 libcgreen1-dev \
      nsis zip dos2unix sshpass && \
    rm -rf /var/lib/apt/lists/*

# Use POSIX-threads for MinGW
RUN update-alternatives --set x86_64-w64-mingw32-gcc /usr/bin/x86_64-w64-mingw32-gcc-posix && \
    update-alternatives --set x86_64-w64-mingw32-g++ /usr/bin/x86_64-w64-mingw32-g++-posix

# win-iconv, once per architecture. Two builds are needed because both
# produce a libwiniconv.a and the makefiles ask for it by name, so they
# cannot share a directory. The 64-bit one serves the SDK cross build;
# the 32-bit one serves winarun, which is 32-bit because WindowsGlk is.
# Each directory keeps its headers, since -I<root> is how iconv.h is found.
#
# Could possibly be replaced by win-iconv-mingw-w64/32-dev...
RUN cd /tmp && \
    git clone https://github.com/win-iconv/win-iconv.git && \
    cp -r win-iconv win-iconv-i686 && \
    cd /tmp/win-iconv && \
    x86_64-w64-mingw32-gcc -O2 -Wall -c win_iconv.c -o win_iconv.o && \
    x86_64-w64-mingw32-ar rcs libwiniconv.a win_iconv.o && \
    x86_64-w64-mingw32-gcc -shared -o winiconv.dll win_iconv.o -Wl,--out-implib,libwiniconv.dll.a && \
    cd /tmp/win-iconv-i686 && \
    i686-w64-mingw32-gcc -O2 -Wall -c win_iconv.c -o win_iconv.o && \
    i686-w64-mingw32-ar rcs libwiniconv.a win_iconv.o && \
    i686-w64-mingw32-gcc -shared -o winiconv.dll win_iconv.o -Wl,--out-implib,libwiniconv.dll.a

ENV WINICONV_ROOT=/tmp/win-iconv
ENV WINICONV32_ROOT=/tmp/win-iconv-i686

RUN cd /tmp && \
    mkdir WindowsGlk && cd WindowsGlk && \
    wget https://github.com/DavidKinder/Windows-Glk/releases/download/1.54/WindowsGlk-154.zip && \
    unzip WindowsGlk*

ENV WINGLK_ROOT=/tmp/WindowsGlk    
    

# Jenkins-user .ssh-directory
RUN useradd -m -d /home/jenkins -s /bin/bash jenkins && \
    install -d -m 700 -o jenkins -g jenkins /home/jenkins/.ssh

USER jenkins
WORKDIR /home/jenkins

# No CMD/ENTRYPOINT — Docker-plugin (Attach) runs the remoting-jar
