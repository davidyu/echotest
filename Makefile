#
##  Makefile for Ray Tracer

MACHINE = $(shell uname -s)

LIBS=
# LIBS = -lpthread

CFLAGS = -O2 -g -std=c++11

CC = g++

INCLUDE =-Isrc/

.SUFFIXES: .o .cpp .cxx

.cpp.o: 
		$(CC) $(CFLAGS) $(INCLUDE) $(FORCE_INCLUDE) -c -o $*.o $<

.cxx.o: 
		$(CC) $(CFLAGS) $(INCLUDE) -c -o $*.o $<

all: server client

client: src/client.o src/SocketAddress.o src/TCPSocket.o
		$(CC) $(CFLAGS) -o $@ src/client.o src/SocketAddress.o src/TCPSocket.o $(INCLUDE) $(LIBDIR) $(LIBS)

server: src/server.o src/SocketAddress.o src/TCPSocket.o
		$(CC) $(CFLAGS) -o $@ src/server.o src/SocketAddress.o src/TCPSocket.o $(INCLUDE) $(LIBDIR) $(LIBS)

ALL.O = src/server.o src/client.o src/SocketAddress.o src/TCPSocket.o

ALL.H = $(wildcard src/*.h)

clean:
		rm -f $(ALL.O)
