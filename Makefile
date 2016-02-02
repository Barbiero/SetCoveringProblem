CPP=g++
CPPFLAGS=-std=c++11 -g $(shell root-config --cflags)
LDLIBS=-g $(shell root-config --ldflags)
FNAME=alggen
SRCDIR=src
SRC=$(SRCDIR)/ioColuna.cpp

$(FNAME): $(SRC)
	$(CPP) $(CPPFLAGS) $(SRC) -o $(FNAME)
