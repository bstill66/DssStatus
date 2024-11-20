FROM debian:latest
RUN apt-get update && \
    apt-get -y install \
    build-essential \
    cmake \
    git \
    clang
RUN g++ --version
RUN clang --version
RUN git clone https://github.com/bstill66/DssStatus.git
WORKDIR /DssStatus
RUN cmake .
RUN make


