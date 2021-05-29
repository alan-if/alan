FROM alanif/gcc-10-cross

LABEL maintainer="Thomas Nilefalk <thomas@alanif.se>"

RUN apt-get update && \
    apt-get -qy full-upgrade && \
    apt-get -qy install libxml-xpath-perl && \
    apt-get -qy install sshpass && \
    mkdir ~/.ssh && \
    ssh-keyscan ssh.alanif.se > ~/.ssh/known_hosts

# Download and setup WindowsGLK
ADD "https://github.com/DavidKinder/Windows-Glk/releases/download/1.51/WindowsGlk-151.zip" /home/jenkins
RUN cd /home/jenkins ; mkdir WindowsGlk ; unzip -d WindowsGlk WindowsGlk-151.zip

# Download and setup Gargoyle