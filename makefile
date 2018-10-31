#!/bin/bash

all: sender receiver

sender:
	g++ -std=c++11 src/ack.cpp src/frame.cpp src/function.cpp src/sender.cpp -o sendfile -lws2_32

receiver:
	g++ -std=c++11 src/ack.cpp src/frame.cpp src/function.cpp src/receiver.cpp -o recvfile -lws2_32

testsend:
	./sendfile data/msg.txt 5 25 127.0.0.1 8888

testrecv:
	./recvfile data/out.txt 5 25 8888