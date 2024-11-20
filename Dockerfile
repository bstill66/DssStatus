FROM alpine:latest as build
RUN apk add g++
RUN apk add cmake
RUN apk add make
RUN apk add git

ADD "https://www.random.org/cgi-bin/randbyte?nbytes=10&format=h" skipcache

RUN git clone https://github.com/bstill66/DssStatus.git
WORKDIR /DssStatus
RUN cmake .
RUN make
