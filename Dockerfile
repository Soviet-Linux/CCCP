# Use the official Debian base image
FROM debian:bullseye

# Set environment variables
ENV DEBIAN_FRONTEND=noninteractive

# Install required packages
RUN apt-get update && apt-get install -y \
    git \
    build-essential \
    libcurl4-gnutls-dev \
    libsqlite3-dev

# Clone the libspm repository and build it
RUN git clone https://github.com/Soviet-Linux/libspm && \
    cd libspm && \
    make all && \
    make formats && \
    make install && \
    make test && \
    cp bin/spm-test /usr/bin/ && \
    cd .. && \
    rm -rf libspm

RUN touch /etc/cccp.conf
    echo "REPOS= https://github.com/Soviet-Linux/OUR/blob/main \nFORMATS= ecmp" >> /etc/cccp.conf

# Set the working directory
WORKDIR /root


# Set the container as an interactive shell
CMD ["bash"]
