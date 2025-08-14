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

# Bas: Java 17, git/ssh, byggkedja, MinGW-w64 64-bit, NSIS, verktyg för libiconv-bygg
RUN apt-get update && apt-get -y upgrade && \
    apt-get install -y --no-install-recommends \
      ca-certificates curl wget unzip xz-utils \
      openjdk-17-jre-headless \
      git openssh-client \
      build-essential make cmake pkg-config gcc g++ \
      gcc-mingw-w64-x86-64 g++-mingw-w64-x86-64 binutils-mingw-w64-x86-64 \
      mingw-w64 mingw-w64-tools \
      nsis zip dos2unix && \
    rm -rf /var/lib/apt/lists/*

# Välj POSIX-trådar för MinGW (oftast det man vill ha)
RUN update-alternatives --set x86_64-w64-mingw32-gcc /usr/bin/x86_64-w64-mingw32-gcc-posix && \
    update-alternatives --set x86_64-w64-mingw32-g++ /usr/bin/x86_64-w64-mingw32-g++-posix

# Bygg och installera GNU libiconv 1.18 för x86_64-w64-mingw32
RUN set -eux; \
    mkdir -p /tmp/libiconv && cd /tmp/libiconv && \
    wget -q https://ftp.gnu.org/pub/gnu/libiconv/libiconv-1.18.tar.gz && \
    tar xf libiconv-1.18.tar.gz && cd libiconv-1.18 && \
    CC=x86_64-w64-mingw32-gcc AR=x86_64-w64-mingw32-ar RANLIB=x86_64-w64-mingw32-ranlib \
      ./configure --host=x86_64-w64-mingw32 --build="$(gcc -dumpmachine)" \
                  --prefix=/usr/local/x86_64-w64-mingw32 && \
    make -j"$(nproc)" && make install && \
    cd / && rm -rf /tmp/libiconv

# Prefix för iconv (enkelt att använda i Makefiles)
ENV ICONV_ROOT=/usr/local/x86_64-w64-mingw32

# Jenkins-användare och .ssh-katalog
RUN useradd -m -d /home/jenkins -s /bin/bash jenkins && \
    install -d -m 700 -o jenkins -g jenkins /home/jenkins/.ssh

USER jenkins
WORKDIR /home/jenkins

# Bekväma env för cross-builds (kan överskuggas i jobben)
ENV CC=x86_64-w64-mingw32-gcc \
    CXX=x86_64-w64-mingw32-g++ \
    AR=x86_64-w64-mingw32-ar \
    STRIP=x86_64-w64-mingw32-strip \
    WINDRES=x86_64-w64-mingw32-windres \
    RANLIB=x86_64-w64-mingw32-ranlib \
    PKG_CONFIG=x86_64-w64-mingw32-pkg-config
# Ingen CMD/ENTRYPOINT — Docker-plugin (Attach) kör remoting-jar själv
