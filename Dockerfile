FROM ubuntu:latest

RUN apt-get update -qq \
	&& apt-get upgrade -y \
	&& apt-get install -y build-essential gdb
