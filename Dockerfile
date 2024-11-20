FROM debian:latest
RUN apt-get update && \
    apt-get -y install \
    build-essential \
    cmake \
    git
RUN g++ --version

ADD "https://www.random.org/cgi-bin/randbyte?nbytes=10&format=h" skipcache
RUN git clone https://github.com/bstill66/DssStatus.git
WORKDIR /DssStatus
RUN cmake .
RUN make


