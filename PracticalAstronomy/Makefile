CC       = gcc
CXX      = g++
CFLAGS   = -Wall -W -Werror
CXXFLAGS = -std=c++17 -Wall -W -Werror
LDFLAGS  =
COBJS    =
CXXOBJS  = main.o misc.o solver.o test.o
OBJS     = $(COBJS) $(CXXOBJS)
SRCS     = $(OBJS:%.o=%.cc)

all: main

main: $(OBJS)
	$(CXX) $(LDFLAGS) $^ -o $@ $(LDLIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	-\rm main $(OBJS)
