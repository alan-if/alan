FROM ubuntu:21.04

LABEL maintainer="Thomas Nilefalk <thomas@alanif.se>"

ENV TZ=Europe/Stockholm
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

# Make sure the package repository is up to date.
RUN apt-get update && \
    apt-get -qy full-upgrade && \
    apt-get install -qy git && \
# Install a basic SSH server
    apt-get install -qy openssh-server && \
    sed -i 's|session    required     pam_loginuid.so|session    optional     pam_loginuid.so|g' /etc/pam.d/sshd && \
    mkdir -p /var/run/sshd && \
# Install JDK 8 (latest stable edition at 2019-04-01)
    apt-get install -qy openjdk-8-jdk && \
# Install build tools, incl. cross for win32
    apt-get install -qy build-essential make gcc-mingw-w64-i686 win-iconv-mingw-w64-dev && \
    apt-get install -qy dos2unix nsis unzip zip nano && \
    apt-get install -qy cgreen1 libcgreen1-dev && \
# Cleanup old packages
    apt-get -qy autoremove && \
# Add user jenkins to the image
    adduser --quiet jenkins && \
# Set password for the jenkins user (you may want to alter this).
    echo "jenkins:jenkins" | chpasswd

RUN service ssh start

# Standard SSH port
EXPOSE 22
CMD ["/usr/sbin/sshd", "-D"]
