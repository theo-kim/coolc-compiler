# This docker file is used to develop the server
FROM ubuntu:18.04 AS coolc
MAINTAINER iggygim
# Install build dependencies
RUN apt-get update -y
RUN apt-get install -y flex bison build-essential nasm colordiff libc6-dev-i386
# Add source directories
RUN mkdir -p /compiler
COPY ./src /compiler/src
COPY ./refimpl /compiler/refimpl
COPY ./cool-examples /compiler/cool-examples
RUN mkdir -p /compiler/tests
RUN mkdir -p /compiler/myimpl
RUN cd /compiler
# Build each layer of the compiler
# Lexer
COPY ./myimpl/lexer /compiler/myimpl/lexer
RUN cd /compiler/myimpl/lexer && make
# Parser
COPY ./myimpl/parser /compiler/myimpl/parser
RUN cd /compiler/myimpl/parser && make
# Semantic Analyzer
COPY ./myimpl/semant /compiler/myimpl/semant
RUN cd /compiler/myimpl/semant && make
# Code generator
COPY ./myimpl/cgen /compiler/myimpl/cgen
RUN cd /compiler/myimpl/cgen && make
# Now load the testing file responsible for running the files
COPY ./docker.sh /compiler/docker.sh
ENTRYPOINT ["/compiler/docker.sh"]