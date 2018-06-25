#
##  Makefile for Ray Tracer

MACHINE = $(shell uname -s)

LIBS=
# LIBS = -lpthread

CFLAGS = -O2 -g -std=c++11

CC = g++

INCLUDE =-Isrc/

# FORCE_INCLUDE=-include rtstd.h

.SUFFIXES: .o .cpp .cxx

.cpp.o: 
		$(CC) $(CFLAGS) $(INCLUDE) $(FORCE_INCLUDE) -c -o $*.o $<

.cxx.o: 
		$(CC) $(CFLAGS) $(INCLUDE) -c -o $*.o $<

# this is necessary! Because it allows Make to understand which files need to be rebuilt
# for ray...
ALL.O = src/server.o

ALL.H = $(wildcard src/*.h)

server: $(ALL.O) $(ALL.H)
		$(CC) $(CFLAGS) -o $@ $(ALL.O) $(INCLUDE) $(LIBDIR) $(LIBS)

clean:
		rm -f $(ALL.O)
