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

all: echoserver echoclient PedanticClient

echoclient: src/echoclient.o src/SocketAddress.o src/TCPSocket.o $(ALL.H)
		$(CC) $(CFLAGS) -o $@ src/echoclient.o src/SocketAddress.o src/TCPSocket.o $(INCLUDE) $(LIBDIR) $(LIBS)

echoserver: src/echoserver.o src/SocketAddress.o src/TCPSocket.o $(ALL.H)
		$(CC) $(CFLAGS) -o $@ src/echoserver.o src/SocketAddress.o src/TCPSocket.o $(INCLUDE) $(LIBDIR) $(LIBS)

PedanticClient: src/PedanticClient.o src/SocketAddress.o src/UDPSocket.o $(ALL.H)
		$(CC) $(CFLAGS) -o $@ src/PedanticClient.o src/SocketAddress.o src/UDPSocket.o $(INCLUDE) $(LIBDIR) $(LIBS)

ALL.O = src/echoserver.o src/echoclient.o src/SocketAddress.o src/TCPSocket.o src/pedanticclient.o src/UDPSocket.o

ALL.H = $(wildcard src/*.h)

clean:
		rm -f $(ALL.O)
